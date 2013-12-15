gdmenu
======

GTK+ dmenu clone.

Dependencies
------------

* gtk+-3.0

Options
-------

* `-t TITLE` Set window title to `TITLE`.
* `-m MAX` Display `MAX` entries at a time.

Controls
--------

* `Esc` `Ctrl-c` Exit.
* `Tab` `Ctrl-i` Set the selected item as the input text.
* `Down` `Ctrl-f` Select the next item.
* `Up` `Ctrl-b` Select the previous item.
* `Return` `Ctrl-j` Output the selected item and exit.
* `Shift-Return` `Ctrl-J` Output the input text and exit.

Installation
------------

```
make && sudo make install
```
