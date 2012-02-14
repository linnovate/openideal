
Localization Update
-------------------
  Automatically download and update your translations fetching them from
  http://localize.drupal.org or any other Localization server.

  The l10n update module helps to keep the translation of your drupal core and
  contributed modules up to date with the central Drupal translation repository
  at http://localize.drupal.org. Alternatively localy stored translation files
  can be used as translation source too.

  By choice updates are performed automatically or manually. Locally altered
  translations can either be respected or ignored.

  The l10n update module is developed for:
   * Distributions which include their own translations in .po files.
   * Site admins who want to update the translation with each new module revision.
   * Site builders who want an easy tool to download translations for a site.
   * Multi-sites that share one translation source.

  Project page:  http://drupal.org/project/l10n_update
  Support queue: http://drupal.org/project/issues/l10n_update

Installation
------------
  Download, unpack and enable the module the usual way.

  Translations status overview can be found at
    Administer > Site building > Translate interface > Update

  Update configuration settings can be found at
    Administer > Site configuration > Languages > Translation updates

Translating Drupal core, modules and themes
-------------------------------------------
  When Drupal core or contributed modules or themes get installed Drupal core
  checks if po translation file are present and update the translation with
  the string found in these files. After this the localization update module
  checks the localization server for more recent translations and updates
  the site translations if a more recent version was found.
  Note that the translations contained in the project packages may become
  obsolete in future releases.

  Using cron translations may be updated regularly. Depending on setting updates
  are performed daily or weekly.

  Changes to translations made locally using the site's build in translation
  interface (Administer > Site building > Translate interface > Search) and
  changes made using the localization client module are marked. Using the
  'Update mode' setting the locally edited string can be kept and not
  overwritten by translation updates.

Alternative sources of translation
----------------------------------

  Each project i.e. modules, themes, etc. can define alternative translation
  servers to retreive the translation updates from.
  Include the following definition in the projects .info file:

    l10n server = example.com
    l10n url = http://example.com/files/translations/l10n_server.xml

  The download path pattern is normally defined in the above defined xml file.
  You may override this path by adding a third definition in the .info file:

    l10n path = http://example.com/files/translations/%core/%project/%project-%release.%language.po

API
---
  Using hook_l10n_servers the l10n update module can be extended to use other
  translation repositories. Which is usefull for organisations who maintain
  their own translation.

  Using hook_l10n_update_projects_alter modules can alter or specify the
  translation repositories on a per module basis.

  See l10n_update.api.txt for more information.

Maintainers
-----------
  Jose Reyero
  Gábor Hojtsy
