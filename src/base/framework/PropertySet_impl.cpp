/*
 * This file is protected by Copyright. Please refer to the COPYRIGHT file 
 * distributed with this source distribution.
 * 
 * This file is part of REDHAWK core.
 * 
 * REDHAWK core is free software: you can redistribute it and/or modify it 
 * under the terms of the GNU Lesser General Public License as published by the 
 * Free Software Foundation, either version 3 of the License, or (at your 
 * option) any later version.
 * 
 * REDHAWK core is distributed in the hope that it will be useful, but WITHOUT 
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or 
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License 
 * for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public License 
 * along with this program.  If not, see http://www.gnu.org/licenses/.
 */


#include <iostream>

#include "ossie/PropertySet_impl.h"
#include "ossie/CorbaUtils.h"
#include <ossie/prop_helpers.h>

PREPARE_LOGGING(PropertySet_impl);

void PropertySet_impl::setExecparamProperties(std::map<std::string, char*>& execparams)
{
    LOG_TRACE(PropertySet_impl, "Setting " << execparams.size() << " exec parameters");

    std::map<std::string, char*>::iterator iter;
    for (iter = execparams.begin(); iter != execparams.end(); iter++) {
        LOG_TRACE(PropertySet_impl, "Property: " << iter->first << " = "
                                              << iter->second);
        const std::string id = iter->first;
        PropertyInterface* property = getPropertyFromId(id);
        // the property can belong to a resource, device, or Device/Domain
        // Manager.  If the property is not found, then it might be a resource
        // property passed through the nodeBooter to the DeviceManager
        if (property) {
            CORBA::Any val = ossie::string_to_any(iter->second,
                    static_cast<CORBA::TCKind> (property->type));
            property->setValue(val);
        } else {
            LOG_WARN(PropertySet_impl, "propery: " << id << " is not defined, ignoring it!!");
        }
    }
    LOG_TRACE(PropertySet_impl, "Done setting exec parameters");
}


PropertySet_impl::~PropertySet_impl ()
{
    // Clean up all property wrappers created by descendents.
    for (std::vector<PropertyInterface*>::iterator ii = ownedWrappers.begin(); ii != ownedWrappers.end(); ++ii) {
        delete *ii;
    }

    // Clean up all property callback functors.
    for (PropertyCallbackMap::iterator ii = propCallbacks.begin(); ii != propCallbacks.end(); ++ii) {
        delete ii->second;
    }
}

void
PropertySet_impl::configure (const CF::Properties& configProperties)
throw (CORBA::SystemException, CF::PropertySet::InvalidConfiguration,
       CF::PropertySet::PartialConfiguration)
{
    TRACE_ENTER(PropertySet_impl);
    boost::mutex::scoped_lock lock(propertySetAccess);

    int validProperties = 0;
    CF::Properties invalidProperties;

    for (CORBA::ULong ii = 0; ii < configProperties.length(); ++ii) {
        PropertyInterface* property = getPropertyFromId((const char*)configProperties[ii].id);
        if (property && property->isConfigurable()) {
            LOG_TRACE(PropertySet_impl, "Configure property: " << property->id);
            try {
                std::vector<std::string>::iterator kind = property->kinds.begin();
                bool sendEvent = false;
                bool eventType = false;
                if (propertyChangePort != NULL) {
                    // searching for event type
                    while (kind != property->kinds.end()) {
                        if (!kind->compare("event")) {
                            // it is of event type
                            eventType = true;
                            break;
                        }
                        kind++;
                    }
                    if (eventType) {
                        // comparing values
                        if (property->compare(configProperties[ii].value)) {
                            // the incoming value is different from the current value
                            sendEvent = true;
                        }
                    }
                }
                property->setValue(configProperties[ii].value);
                executePropertyCallback(property->id);
                if (sendEvent) {
                    // sending the event
                    propertyChangePort->sendPropertyEvent(property->id);
                }
                ++validProperties;
            } catch (std::exception& e) {
                LOG_ERROR(PropertySet_impl, "Setting property " << property->id << " failed.  Cause: " << e.what());
                CORBA::ULong count = invalidProperties.length();
                invalidProperties.length(count + 1);
                invalidProperties[count].id = CORBA::string_dup(configProperties[ii].id);
                invalidProperties[count].value = configProperties[ii].value;
            } catch (CORBA::Exception& e) {
                LOG_ERROR(PropertySet_impl, "Setting property " << property->id << " failed.  Cause: " << e._name());
                CORBA::ULong count = invalidProperties.length();
                invalidProperties.length(count + 1);
                invalidProperties[count].id = CORBA::string_dup(configProperties[ii].id);
                invalidProperties[count].value = configProperties[ii].value;
            }
        } else {
            CORBA::ULong count = invalidProperties.length();
            invalidProperties.length(count + 1);
            invalidProperties[count].id = CORBA::string_dup(configProperties[ii].id);
            invalidProperties[count].value = configProperties[ii].value;
        }
    }

    if (invalidProperties.length () > 0) {
        if (validProperties > 0) {
            throw CF::PropertySet::PartialConfiguration(invalidProperties);
        } else {
            throw CF::PropertySet::InvalidConfiguration("No matching properties found", invalidProperties);
        }
    }

    TRACE_EXIT(PropertySet_impl);
}


void
PropertySet_impl::query (CF::Properties& configProperties)
throw (CORBA::SystemException, CF::UnknownProperties)
{
    TRACE_ENTER(PropertySet_impl);
    boost::mutex::scoped_lock lock(propertySetAccess);

    // For queries of zero length, return all id/value pairs in propertySet.
    if (configProperties.length () == 0) {
        LOG_TRACE(PropertySet_impl, "Query all properties");

        // Handle deprecated old-style properties.
        remapProperties();

        configProperties.length(0);

        PropertyMap::iterator jj = propTable.begin();
        for (CORBA::ULong ii = 0; ii < propTable.size(); ++ii) {
            if (jj->second->isQueryable()) {
                configProperties.length(configProperties.length()+1);
                configProperties[configProperties.length()-1].id = CORBA::string_dup(jj->second->id.c_str());
                if (jj->second->isNilEnabled()) {
                    if (jj->second->isNil()) {
                        configProperties[configProperties.length()-1].value = CORBA::Any();
                    } else {
                        jj->second->getValue(configProperties[configProperties.length()-1].value);
                    }
                } else {
                    jj->second->getValue(configProperties[configProperties.length()-1].value);
                }
            }
            ++jj;
        }
    } else {
        // For queries of length > 0, return all requested pairs in propertySet
        CF::Properties invalidProperties;

        // Returns values for valid queries in the same order as requested
        for (CORBA::ULong ii = 0; ii < configProperties.length (); ++ii) {
            const std::string id = (const char*)configProperties[ii].id;
            LOG_TRACE(PropertySet_impl, "Query property " << id);
            PropertyInterface* property = getPropertyFromId(id);
            if (property && property->isQueryable()) {
                if (property->isNilEnabled()) {
                    if (property->isNil()) {
                        configProperties[ii].value = CORBA::Any();
                    } else {
                        property->getValue(configProperties[ii].value);
                    }
                } else {
                    property->getValue(configProperties[ii].value);
                }
            } else {
                CORBA::ULong count = invalidProperties.length();
                invalidProperties.length(count + 1);
                invalidProperties[count].id = CORBA::string_dup(configProperties[ii].id);
                invalidProperties[count].value = configProperties[ii].value;
            }
        }

        if (invalidProperties.length () != 0) {
            throw CF::UnknownProperties(invalidProperties);
        }
    }

    LOG_TRACE(PropertySet_impl, "Query returning " << configProperties.length() << " properties");

    TRACE_EXIT(PropertySet_impl);
}

propertyContainer* PropertySet_impl::getPropFromId(std::string& id)
{
    TRACE_ENTER(PropertySet_impl);
    std::map<std::string, propertyContainer>::iterator j;
    for (j = propSet.begin(); j != propSet.end(); j++) {
        if ((*j).second.id == id) {
            TRACE_EXIT(PropertySet_impl);
            return &(*j).second;
        }
    }
    TRACE_EXIT(PropertySet_impl);
    return NULL;
}


PropertyInterface* PropertySet_impl::getPropertyFromId (const std::string& id)
{
    // Handle deprecated old-style properties.
    remapProperties();

    PropertyMap::iterator property = propTable.find(id);
    if (property != propTable.end()) {
        return property->second;
    }
    return 0;
}

PropertyInterface* PropertySet_impl::getPropertyFromName (const std::string& name)
{
    // Handle deprecated old-style properties.
    remapProperties();

    for (PropertyMap::iterator property = propTable.begin(); property != propTable.end(); ++property) {
        if (name == property->second->name) {
            return property->second;
        }
    }

    return 0;
}

void
PropertySet_impl::validate (CF::Properties property,
                            CF::Properties& validProps,
                            CF::Properties& invalidProps)
{
    for (CORBA::ULong ii = 0; ii < property.length (); ++ii) {
        std::string id((const char*)property[ii].id);
        if (getPropertyFromId(id)) {
            CORBA::ULong count = validProps.length();
            validProps.length(count + 1);
            validProps[count].id = property[ii].id;
            validProps[count].value = property[ii].value;
        } else {
            CORBA::ULong count = invalidProps.length();
            invalidProps.length(count + 1);
            invalidProps[count].id = property[ii].id;
            invalidProps[count].value = property[ii].value;
        }
    }
}


CF::DataType PropertySet_impl::getProperty (CORBA::String_var _id)
{
    TRACE_ENTER(PropertySet_impl);
    CF::DataType value;
    std::string id = (const char*)(_id);
    PropertyInterface* property = getPropertyFromId(id);
    if (property) {
        value.id = CORBA::string_dup(_id);
        property->getValue(value.value);
    }
    TRACE_EXIT(PropertySet_impl);
    return CF::DataType();
}


void PropertySet_impl::setPropertyChangeListener (const std::string& id, PropertyCallbackFn func)
{
    setPropertyCallback(id, new StaticCallback(func));
}

void PropertySet_impl::executePropertyCallback (const std::string& id)
{
    PropertyCallbackMap::iterator func = propCallbacks.find(id);
    if (propCallbacks.end() == func) {
        return;
    }
    (*func->second)(id);
}

void PropertySet_impl::setPropertyCallback (const std::string& id, PropertyCallback* callback)
{
    std::string propId;

    // Check whether the supplied id is actually a property name; if so, map the name
    // to an id, otherwise assume that 'id' is really a property id.
    PropertyInterface* property = getPropertyFromName(id);
    if (property) {
        propId = property->id;
    } else {
        propId = id;
    }

    if (propCallbacks.count(propId)) {
        delete propCallbacks[propId];
    }
    propCallbacks[propId] = callback;
}

void PropertySet_impl::remapProperties (void)
{
    if ((propSet.size() > 0) && (propSet.size() != propTable.size())) {
        LOG_TRACE(PropertySet_impl, "Remapping properties");
        propTable.clear();
        for (propSet_iterator ii = propSet.begin(); ii != propSet.end(); ++ii) {
            LOG_TRACE(PropertySet_impl, "Adding property " << ii->first);
            propTable[ii->second.id] = &(ii->second);
        }
    }
}
