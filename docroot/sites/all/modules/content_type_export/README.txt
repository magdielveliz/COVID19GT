OVERVIEW
==============
This is a module for module developers.
It aims to speed up module development by making it easier to create bundles and
fields though the UI and then export them to code to put into a modules install
file or update hook.
This module should not be enabled on production servers.


FUNCTIONALITY
==============
This module adds an export tab to entity edit pages allowing you to export field
and field instance definitions.

The export page works for the following entity types:
  - Node
      Home » Administration » Structure » Content types » {YOUR_BUNDLE} » Export
  - Entity form types
      Home » Administration » Structure » Entityform Types » {YOUR_ENTITY_FORM_TYPE}  » Export
  - Taxonomy
      Home » Administration » Structure » Taxonomy » {YOUR_TAXONOMY} » Export
  - Field collection
  - Paragraphs
  - Commerce product types
  - Entity form types
  - Beans

It is also possible to export individual fields attached to commerce customer
profile types. Edit the field definition and an export tab will be there.

The export page has the following code snippets:
  - Content type
      Code to create the bundle for this entity type.
  - Fields
      Code to create all the fields attached this this bundle.
  - Instances
      Code to create all field instances for this bundle.
  - Uninstall code
      Code to remove this bundle and all it's fields.
  - The whole module.install
      Copying this code to your modules install file and replacing MY_MODULE
      with the name of your module will be all you need to do to create the
      given bundle and create and attach all the fields.


Exporting individual fields
---------------------------
Individual fields can also be exported by visiting the fields edit page and
clicking on the export tab. This page gives code to A) create the field, B)
create the field instance and C) update an existing instance with new
configuration.


SUB MODULES
==============

Image style to code
-------------------
This allows you to export your image style to code that can be run in a modules
hook_update_N function to recreate the image style and effects.
To export an image style visit the image styles edit page. At the top of the
page there is a button "Export image style to code".
This take you to a page with two text areas. The first shows the code required
to create the image style and effects and the second shows the code required to
delete the image style (for a modules uninstall function).
If an image style already exists with the same name then it will update the
existing style.

Node to code
-------------------
Adds an export tab to a node page. This export page shows the code to create the
current node again programmatically.
This may not work with all field types. Always test before pushing to a
production environment.


View mode to code
-------------------
The definition of a view mode is split between all of the field instances that
are attached to that bundle. This module creates code that updates all field
instances attached to a given bundle for a specific view mode.
To use visit the mange display page - at the bottom of the page next to the save
button is an export button. Clicking this takes you to the export page with the
code required to update all the display modes for each field instance.
Note this code will only update existing field instances - not create them.
