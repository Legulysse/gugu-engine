# Databinding

See also :
- [Datasheets](../Features/Datasheets.md)
- [Datasaves](../Features/Datasaves.md)

## Design goals

Declaring and implementing data objects that are serializable and editable by both designers and gameplay programmers is often boring, time consuming, and can easily lead to a lot of maintenance problems.

The goal of the databinding system is to provide a way to easily iterate on the gameplay code and its associated gameplay database, for both programmers and designers, by using code generation and a dynamic editor to reduce implementation efforts to its bare minimum.  

It provides two types of data :
- Datasheets : static database objects, handle polymorphism and inheritance.
- Datasaves : runtime serialization objects, handle polymorphism.

Datasheets are the main component of this system, providing a dedicated Editor for designers to manipulate complex database objects, that can be deserialized and used in gameplay code as static objects.  
It handles inheritance (a datasheet can inherit its content from a parent datasheet), polymorphism (an instance property can be instanciated using any subtype available), and references between datasheets.

Datasaves are a complementary type of data, taking advantage of most of the features developped for the datasheets, with the intention of being serialized/deserialized and updated at runtime, providing a strong basis for a save system.

## Specifications
- All data files are simple, flat and deterministic xml files (avoid as many merge conflicts as possible, files are always editable by hand if needed).
- Both programmers and designers can declare classes/enums (xml definitions).
- The Editor UI for datasheets is generated at runtime (no compilation needed).
- On the programming side, classes are generated with explicit and readable code (breakpoint-friendly, no manual declaration needed for classes and serializers).
- Deserialization is automatically handled at runtime (get a fully deserialized datasheet by simply providing its filename).
- Deserialized data objects are strongly typed, with autocompletion available in the IDE.
- Serialization (datasaves) is available for all data objects (just instanciate a hierarchy of objects and call the Save method on its root).

## Tools

A standalone Editor is available to provide a UI for datasheets edition.  
This Editor will generate its interfaces at runtime by parsing the provided xml definition of the project, allowing designers to directly edit datasheets without any compilation needed.

A python script is available to generate C++ code declarations (classes, enums, factories) from the provided xml definition.  
Other languages or engines could be provided as extensions for this script.

At the time of writing, the generic code needed to fully deserialize datasheets at runtime is only provided for my own gugu::Engine (C++), but other languages or engines could be provided too (You can contact me to discuss this).
