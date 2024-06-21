# Datasheets

## Description

Datasheets are used as static database objects.

## Available Features

- Classes : Classes can be defined as a collection of properties.
- Base types : Properties can use several base types (bool, int, float, string, vector2i, vector2f).
- Enums : Enums can be declared in addition to base types.
- References : A property can be a reference to an other datasheet (From a defined base class).
- Instances : A property can be an instance and hold a sub-object (Using any subclass from a defined base class).
- Arrays : A property of any type can be declared as an array (base type, enum, instance, reference).
- Class Inheritance : A defined class can inherit a base class, inheriting all its property definitions.
- Data Inheritance : A datasheet can inherit from a parent datasheet, and inherit its property values. (All its properties will default on its parent properties, then fallback on this parent's parent if it did not provide values for those properties, etc. until going back to the class defaults).
- Overrides : A datasheet inheriting from a base datasheet will only serialize its override values, and default to its parent properties by default (All its properties will default on its parent override properties, then fallback on this parent's parent if it did not provide overrides for those properties, etc. until going back to the class defaults).
- Nested instance properties overrides : A datasheet inheriting a base datasheet can handle propery overrides on its root object, but also on any subobject (instances) inside its hierarchy.
- Polymorphism : A property defined as an instance for a defined class can hold an object from any subclass available matching this base class.


## Example

### Binding

```xml
<Class name="item" code="DS_Item">
    <Data type="string" name="name" />
    <Data type="string" name="icon" />
    <Data type="int" name="price" />
</Class>
```

### Generated Code

```cpp
class DS_Item : public gugu::DatasheetObject
{
    std::string name;
    std::string icon;
    int price;
    
    // + ctor/dtor and deserialize methods
};
```

### Gameplay Code

```cpp
const DS_Item* itemWingBoots = GetResources()->GetDatasheetObject<DS_Item>("WingBoots.item");
std::string itemName = itemWingBoots->name; // = "Wing Boots"
std::string itemIcon = itemWingBoots->icon; // = "Boots.png"
int itemPrice = itemWingBoots->price;       // = 1500
```

