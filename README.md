# GameObjectListEditor
GameObjectListEditor (or just ListEditor) is a little document editing application which allows you to create list of objects: choose their names, properties and description.

## First Look

As you can see, application sepparactes into 2 main parts: Object Types Area and Edit Area.

Object Types Area sugests you base types to fill your documents. They can't be deleted, cause they placed in resource file of project. Nevertheless, you can add your own types for current session.
Edit Area allows you to create new object types and add new data into your document.

![alt text](https://github.com/Den-One/GameObjectListEditor/blob/main/documents/first_look.png)

## Create Type

Let's add a new object into our type list. We just click on 'Create type' button to see in the Edit Area a menu of type creator.

![alt text](https://github.com/Den-One/GameObjectListEditor/blob/main/documents/push-create_type.png)

Let's fill in the textEdits and textAreas. Object and property name should be a single world or a group with no spaces. Otherwise the application will unite them.
If you wanna add less then 2 properties, simply remain the areas unforfilled. Object name LineEdit should have text or object types won't be created.
Just push "Save" button to add new object into Type List.

![alt text](https://github.com/Den-One/GameObjectListEditor/blob/main/documents/create_type.png)

New object types saves sepparatly out of base object types and will be removed when application will be cloused.

## Change Existing List

So let's apply these changes to our pre-created list. Choose 'Open List...' option from menubar on the top.

![alt text](https://github.com/Den-One/GameObjectListEditor/blob/main/documents/menubar-open_list.png)

In opening dialog window it's already setted to recognise only .txt files. Choose the testFile.txt to open it.

![alt text](https://github.com/Den-One/GameObjectListEditor/blob/main/documents/choose_file.png)

If we look into out testFile.txt, it has a lot of raw data. But don't worry, the application parses all the information before displaying it.

![alt text](https://github.com/Den-One/GameObjectListEditor/blob/main/documents/raw-dataFile.png)

Now we can add out object into list. Object List consists of buttons so just click on one. It'll compare its title with Parsed data to choose the right properties for new item in the Edit Area.

![alt text](https://github.com/Den-One/GameObjectListEditor/blob/main/documents/add-object_type.png)

List Editor allows you to add pre-created types into list too, so let's add a couple.

![alt text](https://github.com/Den-One/GameObjectListEditor/blob/main/documents/form-list.png)

Our list looks finished. Now it's time to save our changes. Just click on Save file option.

![alt text](https://github.com/Den-One/GameObjectListEditor/blob/main/documents/munebar-saveList.png)

## Create New List

To create a new list, choose "New List..." in the menubar.

![alt text](https://github.com/Den-One/GameObjectListEditor/blob/main/documents/munebar-new_list.png)

It opens a small window to type the name. It shouldn't contan spaces. It's already suggests you .txt extenshion.

![alt text](https://github.com/Den-One/GameObjectListEditor/blob/main/documents/enterFileName.png)

After then you need to choose the directory to locate your new file.

![alt text](https://github.com/Den-One/GameObjectListEditor/blob/main/documents/choose_folder.png)

## Credits

Thanks to Lesta Studio for abbility to apply my knowledge in C++ via this test task. It was so fun codying that. I would like to believe that the sleepless nights were not in vain.