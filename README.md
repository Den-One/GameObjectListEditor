# GameObjectListEditor
GameObjectListEditor (or just ListEditor) is a little document editing application which allows you to create lists of objects: choose their names, properties and description.

## Installation

To download and run this application, click on the button below

<!-- BEGIN LATEST DOWNLOAD BUTTON -->
[![Download zip](https://custom-icon-badges.demolab.com/badge/-Download-blue?style=for-the-badge&logo=download&logoColor=white "Download zip")](https://github.com/Den-One/GameObjectListEditor/blob/main/GameObjectListEditor.zip)
<!-- END LATEST DOWNLOAD BUTTON -->

## Hot Keys
| Hot Key | Desription                                               |
| :-----: | -------------------------------------------------------- |
| Ctrl+N  | Creates a new empty file.                                |
| Ctrl+O  | Opens an earlier created file.                           |
| Ctrl+S  | Saves opened file changes. Disables undo\redo options.   |
| Ctrl+Z  | Undo changes.                                            |
| Ctrl+Y  | Redo changes.                                            |

## Main Window States
|        | start | create | view | change |
| :----: | :---: | :----: | :--: | :----: |
| start  |       | +      | +    | +      |
| create | +     |        |      |        |
| view   |       | +      |      | +      |
| change |       |        | +    |        |

## First Look

As you can see, application separates into 2 main parts: Object Types Area & Edit Area.

Object Types Area offers you base types to fill your documents. Right know they are not clickable, 'cause we need to open or create a document.
These types can't be deleted, since they placed in resource file of project. Nevertheless, you can add your own types for current session.
Edit Area allows you to create new object types and add new data in your document.

![alt text](https://github.com/Den-One/GameObjectListEditor/blob/main/documents/first_look.png)

## Create Type

Let's add a new object into our type list. We just click on 'Create type' button to see the menu of Type Creator in the Edit Area.

![alt text](https://github.com/Den-One/GameObjectListEditor/blob/main/documents/push-create_type.png)

Let's fill in the textEdits and textAreas. Names should be one string with no spaces. Otherwise, application will unite them, anyway. It was made to simplify parsing.
If you wanna add less than 2 properties, simply remain the areas unfulfilled. When you finish, just push "Save or quit" button to add new object into Type List. You will quit Type Creator with no changes, if stay lineEdits unforfilled.

![alt text](https://github.com/Den-One/GameObjectListEditor/blob/main/documents/create_type.png)

New object types save separately from the base object types and will be removed when the application is closed.

## Change Existing List

So let's apply these changes to our pre-created list. Select the 'Open List...' option from the menubar at the top.

![alt text](https://github.com/Den-One/GameObjectListEditor/blob/main/documents/menubar-open_list.png)

The dialog window that opens is already configured to recognize only .txt files. Choose produced by this application *.txt files to open it.

![alt text](https://github.com/Den-One/GameObjectListEditor/blob/main/documents/choose_file.png)

If we look into out testFile.txt, it has a lot of raw data. The application parses all the information before displaying it.
While parsing, empty lines are ignored. The document has 2 types of lines: 'OBJECT' and 'PROPERTY'. After this keyword comes the name of the entity. A property type string has a property name and description. A string of type object has only a name. Objects can have null properties.

![alt text](https://github.com/Den-One/GameObjectListEditor/blob/main/documents/raw-dataFile.png)

Now we can add our object into list. The List of object consists of buttons, so just click on one. It'll compare its title with the parsed data to select the correct properties for the new item in the Edit Area.

![alt text](https://github.com/Den-One/GameObjectListEditor/blob/main/documents/add-object_type.png)

The List Editor also allows us to add pre-created types to our set, so let's add a couple.

![alt text](https://github.com/Den-One/GameObjectListEditor/blob/main/documents/form-list.png)

Our list looks finished. Now it's time to save our changes. Just click on the "Save file" option.

![alt text](https://github.com/Den-One/GameObjectListEditor/blob/main/documents/menubar-saveList.png)

## Create New List

To create a new list, choose "New List..." in the menubar.

![alt text](https://github.com/Den-One/GameObjectListEditor/blob/main/documents/menubar-new_list.png)

It opens a small window to type the name. It shouldn't contan spaces. It already suggests you .txt extenshion.

![alt text](https://github.com/Den-One/GameObjectListEditor/blob/main/documents/enterFileName.png)

After that, you need to select a directory to place the new file.

![alt text](https://github.com/Den-One/GameObjectListEditor/blob/main/documents/choose_folder.png)

When it starts, list is empty. Display may look like we opened the application just now, but now buttons in the Types Area are clickable. We can easily change our last action via Undo and Redo options.

![alt text](https://github.com/Den-One/GameObjectListEditor/blob/main/documents/undo-option.png)

The data about new objects saves into stack. When you choose 'Undo' option, it leave that stack to get into another one, so you'll be able to redo action.
By the way, the undoStack data also used for saving document. It writes to file starting from the end.

![alt text](https://github.com/Den-One/GameObjectListEditor/blob/main/documents/list-editing.png)

## Credits

Thanks to Lesta Studio for the opportunity to apply my C++ knowledge through this test task. There was so much fun to code. I'd like to believe that the sleepless nights were not in vain.