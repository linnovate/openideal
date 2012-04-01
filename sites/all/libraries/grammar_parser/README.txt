
CONTENTS OF THIS FILE
---------------------

 * Author
 * Description
 * Installation
 * Interfaces
 * Drupal versions
 * Developers

AUTHOR
------
Jim Berry ("solotandem", http://drupal.org/user/240748)

DESCRIPTION
-----------
This library module provides a recursive descent grammar parser to help analyze
and modify a source code file. The goal is that, by organizing the source code
based on the grammar of the programming language, complex changes to the code
can be more readily made in a programmatic fashion (i.e. by other code using the
parser engine).

This library handles PHP grammar, building on the tokenizer functions available
in PHP.

INSTALLATION
------------
Although the code repository is hosted on drupal.org, this code library is
independent of Drupal and may be utilized outside of a Drupal context. The
downloads available on drupal.org package it as a 7.x module.

In the context of Drupal, this project is available as:
- a library using the Libraries API (2.x-dev), and
- a Drupal module (the Drupal project API does not offer a library type).

If used as a library, then install it in a libraries directory (e.g.,
"sites/all/libraries" or equivalent) but do not "enable" it as a module.

If used as a module, then install it in a modules directory. See
http://drupal.org/node/895232 for further information.

INTERFACES
----------
A user interface is provided by the Grammar Parser UI module available at
http://drupal.org/project/grammar_parser_ui. The interface allows you to specify
the code to be parsed as individual files, entire directories, and inline code
entered in a text field.

A library interface is provided by the Grammar Parser Library module available at
http://drupal.org/project/grammar_parser_lib. This interface enables automatic
loading of the classes defined in the code. The Drush Make file include with
this project will install this code library for use with the Libraries API.

DRUPAL VERSIONS
---------------
The library is not specific to a Drupal version, but is more likely specific to
a PHP library version. When used as a module, the 7.x download can be used with
any version of Drupal simply by changing the version string in the info file.
For use with Drupal 6 with Drush Make, see http://drupal.org/node/994518.

DEVELOPERS
----------
In the event of issues with the parser, debug output may be enabled on the
settings page of the Grammar Parser UI module. It is recommended to enable this
only with smaller files that include the code causing an issue.
