Image style to code
===================

This allows you to export your image style to code that can be run in a modules
hook_update_N function to recreate the image style and effects.
To export an image style visit the image styles edit page. At the top of the
page there is a button "Export image style to code".
This take you to a page with two text areas. The first shows the code required
to create the image style and effects and the second shows the code required to
delete the image style (for a modules uninstall function).
If an image style already exists with the same name then it will update the
existing style.
