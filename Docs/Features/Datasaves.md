# Datasaves

## Description

Datasaves are classes defined in a data binding xml file, used for serialized data at runtime.  
The xml definitions are parsed to generate c++ code (classes definition, serialization methods, and factories).  
Those data can be used as data holders by gameplay classes, by simply providing your gameplay entities a reference to the data they can manipulate internally, from the game save hierarchy.

## Example

### Binding

```xml
<Class name="playerSave" code="DS_PlayerSave" type="datasave">
    <Data type="reference:player" name="playerSheet" />
    <Data type="int" name="stamina" />
    <Data type="instance:inventorySave" name="inventory" />
</Class>

<Class name="gameSave" code="DS_GameSave" type="datasave">
    <Data type="instance:playerSave" name="player" />
</Class>
```

### Generated Code

```cpp
class DS_PlayerSave : public gugu::DatasaveObject
{
    const DS_Player* playerSheet;   // reference on a datasheet
    int stamina;                    // gameplay property
    DS_InventorySave* inventory;    // instance handling inventory properties
    
    // + serialize/deserialize methods
};

class DS_GameSave : public gugu::DatasaveObject
{
    DS_PlayerSave* player;

    // + serialize/deserialize methods
};
```

### Gameplay Code

```cpp
class PlayerSceneCharacter 
{
    DS_PlayerSave* m_data;    // reference on serialized data, actually owned by the game save
        
    void UseStamina(int value)
    {
        m_data->stamina -= value;
    }

    int GetStamina() const
    {
        return m_data->stamina;
    }
};

// Loading
DS_GameSave gameSave = new DS_GameSave;
gameSave->LoadFromFile("User/Save.xml");

PlayerSceneCharacter character = new PlayerSceneCharacter ;
character->m_data = gameSave->playerSave;

// Saving
gameSave->SaveToFile("User/Save.xml");
```