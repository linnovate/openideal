A general logging utility that can be used as (yet another) activity module.

The main differences between message module and activity module are:

* In message module, the arguments of a sentence aren't hard-coded. This means 
  that the rendering time is slower than activity, on the other hand you can use 
  callback functions to render the final output (see message_example module).
* Thanks to the dependency on the Entity API, the messages are exportable and
  integrated with the Features module.
* Message integrates with i18n, so you can translate your messages (enable 
  i18strings module).
* Message can use (but not as a dependency) the Rules module, to create message 
  instances via the "Entity create" action, whereas the text replacement
  arguments can be set via the "Set data value" action.
* For displaying messages, the modules comes with Views support.
