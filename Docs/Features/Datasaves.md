# Datasaves

## Description

Datasaves are used for serializing data at runtime.  

Datasave classes are defined in a databinding xml definition file.  
The xml definitions are parsed to generate c++ code (classes definition, serialization methods, and factories).  
The declared classes can then be instanciated and manipulated directly by the gameplay code, and will provide a set of load/save methods.  
They will handle nested object instances, as well as references to datasheets.

## Example

### Binding

```xml
<Class name="gameSave" code="DS_GameSave" type="datasave">
    <Data type="instance:playerSave" name="player" />
</Class>

<Class name="playerSave" code="DS_PlayerSave" type="datasave">
    <Data type="int" name="money" />
    <Data type="array:instance:itemSave" name="inventory" />
</Class>

<Class name="itemSave" code="DS_ItemSave" type="datasave">
    <Data type="reference:item" name="item" />
    <Data type="int" name="quantity" />
</Class>
```

### Generated Code

```cpp
class DS_GameSave : public gugu::DatasaveObject
{
    DS_PlayerSave* player;      // player save instance
    
    // + ctor/dtor and serialize/deserialize methods
};

class DS_PlayerSave : public gugu::DatasaveObject
{
    int money;                              // gameplay property
    std::vector<DS_ItemSave*> inventory;    // array of item instances
    
    // + ctor/dtor and serialize/deserialize methods
};

class DS_ItemSave : public gugu::DatasaveObject
{
    const DS_Item* item;        // reference to a datasheet
    int quantity;               // gameplay property
    
    // + ctor/dtor and serialize/deserialize methods
};
```

### Gameplay Code

```cpp
// New save
DS_GameSave* newGameSave = new DS_GameSave;
newGameSave->player = new DS_PlayerSave;

// Saving
newGameSave->SaveToFile("User/Save.xml");

// Loading
DS_GameSave* gameSave = new DS_GameSave;
gameSave->LoadFromFile("User/Save.xml");

// Gameplay
DS_ItemSave* itemSave = new DS_ItemSave;
itemSave->item = nullptr;
itemSave->quantity = 10;

gameSave->player->money += 250;
gameSave->player->inventory.push_back(itemSave);

// Call the SaveToFile again anytime
```

### Gameplay Code (Further Integration)

To facilitate iterations between gameplay code and serialization, a suggested approach is to give game objects runtime instances a reference to their associated serialized data.  
This way the gameplay code can directly manipulate this data, and the central loop only has to call the SaveToFile method without the need to parse and collect game objects data.

```cpp
class PlayerSceneCharacter
{
private:

    DS_PlayerSave* m_data;    // reference to serialized data, actually owned by the game save

public:

    PlayerSceneCharacter(DS_PlayerSave* data)
    {
        m_data = data;
    }

    void ReceiveMoney(int value) const
    {
        m_data->money += value;
    }

    void ReceiveItem(DS_ItemSave* item)
    {
        m_data->inventory.push_back(item);
    }
};
```