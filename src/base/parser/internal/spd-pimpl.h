// Not copyrighted - public domain.
//
// This sample parser implementation was generated by CodeSynthesis XSD,
// an XML Schema to C++ data binding compiler. You may use it in your
// programs without any restrictions.
//

#ifndef CXX___XML_XSD_SPD_PIMPL_H
#define CXX___XML_XSD_SPD_PIMPL_H

#include "spd-pskel.h"

namespace spd
{
  class softPkg_pimpl: public softPkg_pskel
  {
    public:
    virtual void
    pre ();

    virtual void
    title (const ::std::string&);

    virtual void
    author (const ossie::SPD::Author&);

    virtual void
    description (const ::std::string&);

    virtual void
    propertyfile (const std::string&);

    virtual void
    descriptor (const ::std::string&);

    virtual void
    implementation (const ossie::SPD::Implementation&);

    virtual void
    usesdevice (const ossie::SPD::UsesDevice&);

    virtual void
    id (const ::std::string&);

    virtual void
    name (const ::std::string&);

    virtual void
    type (const ::std::string&);

    virtual void
    version (const ::std::string&);

    virtual std::auto_ptr<ossie::SPD>
    post_softPkg ();

    private:
    std::auto_ptr<ossie::SPD> _spd;
  };

  class localFile_pimpl: public virtual localFile_pskel
  {
    public:
    virtual void
    pre ();

    virtual void
    name (const ::std::string&);

    virtual ::std::string 
    post_localFile ();

    public:
    std::string localfile;
  };

  class propertyFile_pimpl: public virtual propertyFile_pskel
  {
    public:
    virtual void
    pre ();

    virtual void
    localfile (const ::std::string&);

    virtual void
    type (const ::std::string&);

    virtual std::string 
    post_propertyFile ();
    
    private:
    std::string propertyfile;
  };

  class author_pimpl: public virtual author_pskel
  {
    public:
    virtual void
    pre ();

    virtual void
    name (const ::std::string&);

    virtual void
    company (const ::std::string&);

    virtual void
    webpage (const ::std::string&);

    virtual ossie::SPD::Author
    post_author ();

    private:
    std::auto_ptr<ossie::SPD::Author> author;
  };

  class descriptor_pimpl: public virtual descriptor_pskel
  {
    public:
    virtual void
    pre ();

    virtual void
    localfile (const ::std::string&);

    virtual void
    name (const ::std::string&);

    virtual ::std::string
    post_descriptor ();
    
    private:
    std::string _localfile;
  };

  class implementation_pimpl: public virtual implementation_pskel
  {
    public:
    virtual void
    pre ();

    virtual void
    description (const ::std::string&);

    virtual void
    propertyfile (const ::std::string&);

    virtual void
    code (const ossie::SPD::Code&);

    virtual void
    compiler (const ossie::SPD::NameVersionPair&);

    virtual void
    programminglanguage ();

    virtual void
    humanlanguage ();

    virtual void
    runtime (const ossie::SPD::NameVersionPair&);

    virtual void
    os (const ossie::SPD::NameVersionPair&);

    virtual void
    processor (const ::std::string&);

    virtual void
    dependency (ossie::SPD::DependencyRef*);

    virtual void
    usesdevice (const ossie::SPD::UsesDevice&);

    virtual void
    id (const ::std::string&);

    virtual void
    aepcompliance ();

    virtual ossie::SPD::Implementation
    post_implementation ();

    private:
    std::auto_ptr<ossie::SPD::Implementation> implementation;
  };

  class code_pimpl: public virtual code_pskel
  {
    public:
    virtual void
    pre ();

    virtual void
    localfile (const ::std::string&);

    virtual void
    entrypoint (const ::std::string&);

    virtual void
    stacksize (unsigned long long);

    virtual void
    priority (unsigned long long);

    virtual void
    type (const ::std::string&);

    virtual ossie::SPD::Code
    post_code ();

    private:
    std::auto_ptr<ossie::SPD::Code> code;
  };

  class compiler_pimpl: public virtual compiler_pskel
  {
    public:
    virtual void
    pre ();

    virtual void
    name (const ::std::string&);

    virtual void
    version (const ::std::string&);

    virtual ossie::SPD::NameVersionPair
    post_compiler ();

    private:
    ossie::SPD::NameVersionPair compiler;
  };

  class programmingLanguage_pimpl: public virtual programmingLanguage_pskel
  {
    public:
    virtual void
    pre ();

    virtual void
    name (const ::std::string&);

    virtual void
    version (const ::std::string&);

    virtual void
    post_programmingLanguage ();
  };

  class humanLanguage_pimpl: public virtual humanLanguage_pskel
  {
    public:
    virtual void
    pre ();

    virtual void
    name (const ::std::string&);

    virtual void
    post_humanLanguage ();
  };

  class os_pimpl: public virtual os_pskel
  {
    public:
    virtual void
    pre ();

    virtual void
    name (const ::std::string&);

    virtual void
    version (const ::std::string&);

    virtual ossie::SPD::NameVersionPair
    post_os ();
    
    private:
    ossie::SPD::NameVersionPair os;
  };

  class processor_pimpl: public virtual processor_pskel
  {
    public:
    virtual void
    pre ();

    virtual void
    name (const ::std::string&);

    virtual ::std::string
    post_processor ();

    private:
    std::string processor;
  };

  class dependency_pimpl: public virtual dependency_pskel
  {
    public:
    virtual void
    pre ();

    virtual void
    softpkgref (const ossie::SPD::SoftPkgRef& ref);
 
    virtual void
    propertyref (const ossie::SPD::PropertyRef&);

    virtual void
    simpleref (ossie::SimplePropertyRef*);

    virtual void
    simplesequenceref (ossie::SimpleSequencePropertyRef*);

    virtual void
    structref (ossie::StructPropertyRef*);

    virtual void
    structsequenceref (ossie::StructSequencePropertyRef*);

    virtual void
    type (const ::std::string&);

    virtual ossie::SPD::DependencyRef*
    post_dependency ();

    private:
    std::auto_ptr<ossie::SPD::DependencyRef> _ref;
  };

  class runtime_pimpl: public virtual runtime_pskel
  {
    public:
    virtual void
    pre ();

    virtual void
    name (const ::std::string&);

    virtual void
    version (const ::std::string&);

    virtual ossie::SPD::NameVersionPair
    post_runtime ();

    private:
    ossie::SPD::NameVersionPair runtime;
  };

  class propertyRef_pimpl: public virtual propertyRef_pskel
  {
    public:
    virtual void
    pre ();

    virtual void
    refid (const ::std::string&);

    virtual void
    value (const ::std::string&);

    virtual ossie::SPD::PropertyRef
    post_propertyRef ();

    private:
    std::auto_ptr<ossie::SimplePropertyRef> _ref;
  };

  class softPkgRef_pimpl: public virtual softPkgRef_pskel
  {
    public:
    virtual void
    pre ();

    virtual void
    localfile (const ::std::string&);

    /*virtual void
    implref ();*/
    virtual void
    implref (const ::std::string&);

    virtual ossie::SPD::SoftPkgRef
    post_softPkgRef ();

    private:
    ossie::SPD::SoftPkgRef _ref;
  };

  class implRef_pimpl: public virtual implRef_pskel
  {
    public:
    virtual void
    pre ();

    virtual void
    refid (const ::std::string&);

    virtual ::std::string
    post_implRef ();

    private:
    ::std::string _refid;
  };

  class usesDevice_pimpl: public virtual usesDevice_pskel
  {
    public:
    virtual void
    pre ();

    virtual void
    propertyref (const ossie::SPD::PropertyRef&);

    virtual void
    simpleref (ossie::SimplePropertyRef*);

    virtual void
    simplesequenceref (ossie::SimpleSequencePropertyRef*);

    virtual void
    structref (ossie::StructPropertyRef*);

    virtual void
    structsequenceref (ossie::StructSequencePropertyRef*);

    virtual void
    id (const ::std::string&);

    virtual void
    type (const ::std::string&);

    virtual ossie::SPD::UsesDevice
    post_usesDevice ();

    private:
    std::auto_ptr<ossie::SPD::UsesDevice> _uses;
  };

  class scaComplianceType_pimpl: public virtual scaComplianceType_pskel,
    public ::xml_schema::nmtoken_pimpl
  {
    public:
    virtual void
    pre ();

    virtual ::std::string
    post_scaComplianceType ();
  };

  class aepcompliance_pimpl: public virtual aepcompliance_pskel,
    public ::xml_schema::nmtoken_pimpl
  {
    public:
    virtual void
    pre ();

    virtual void
    post_aepcompliance ();
  };

  class codeFileType_pimpl: public virtual codeFileType_pskel,
    public ::xml_schema::nmtoken_pimpl
  {
    public:
    virtual void
    pre ();

    virtual ::std::string
    post_codeFileType ();
  };

  class simpleref_pimpl: public virtual simpleref_pskel
  {
    public:
    virtual void
    pre ();

    virtual void
    refid (const ::std::string&);

    virtual void
    value (const ::std::string&);

    virtual ossie::SimplePropertyRef*
    post_simpleref ();
    
    private:
    ossie::SimplePropertyRef* simple;
  };

  class simplesequenceref_pimpl: public virtual simplesequenceref_pskel
  {
    public:
    virtual void
    pre ();

    virtual void
    values (const ::std::vector<std::string>&);

    virtual void
    refid (const ::std::string&);

    virtual ossie::SimpleSequencePropertyRef*
    post_simplesequenceref ();
    private:
    ossie::SimpleSequencePropertyRef* simpleseq;
  };

  class structref_pimpl: public virtual structref_pskel
  {
    public:
    virtual void
    pre ();

    virtual void
    simpleref (ossie::SimplePropertyRef*);

    virtual void
    refid (const ::std::string&);

    virtual ossie::StructPropertyRef*
    post_structref ();

    private:
    ossie::StructPropertyRef* structref;
  };

  class structsequenceref_pimpl: public virtual structsequenceref_pskel
  {
    public:
    virtual void
    pre ();

    virtual void
    structvalue (const ::std::map<std::string, std::string>&);

    virtual void
    refid (const ::std::string&);

    virtual ossie::StructSequencePropertyRef*
    post_structsequenceref ();

    private:
    ossie::StructSequencePropertyRef* structsequenceref;
  };

  class structvalue_pimpl: public virtual structvalue_pskel
  {
    public:
    virtual void
    pre ();

    virtual void
    simpleref (ossie::SimplePropertyRef*);

    virtual ::std::map<std::string, std::string>
    post_structvalue ();

    private:
    std::map<std::string, std::string> values;
  };

  class values_pimpl: public virtual values_pskel
  {
    public:
    virtual void
    pre ();

    virtual void
    value (const ::std::string&);

    virtual ::std::vector<std::string>
    post_values ();

    private:
    ::std::vector<std::string> values;
  };

}

#endif // CXX___XML_XSD_SPD_PIMPL_H
