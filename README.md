# OpenideaL - ideas and innovation management system


![OpenideaL logo](https://www.openidealapp.com/wp-content/uploads/2018/02/logo_OpenideaL.png)



[![Twitter Follow](https://img.shields.io/twitter/follow/openideal?label=Follow%20%40OpenideaL%20on%20Twitter)](https://twitter.com/intent/follow?screen_name=openideal)
[![GitHub forks](https://img.shields.io/github/forks/linnovate/openideal?label=Fork%20OpenideaL%20on%20Github)](https://github.com/linnovate/openideal/fork)
[![Packagist Version](https://img.shields.io/packagist/v/linnovate/openideal-composer.svg)](https://packagist.org/packages/linnovate/openideal-composer)

## Overview

**OpenideaL** (OI) is the leading open source ideas and innovation management system.

Out of the box it provides an ideation community where citizens, employees, clients or any other group of stake holders can create, discuss, rate and promote ideas. 

Since 2010 OpenideaL is in use by various organizations including multi-national and top-500  companies, governments, cities, universities and NGOs. 

OpenideaL includes tools for the website managers which allow them to identify *successful* ideas (those ideas which have a better chance to be relaized), and to pass them along to professional teams within the organization. Community members are rewarded with points for their activity in the system (creating ideas, participating in discussions etc.)

OI is based on Drupal, and therefore it is modular, and allows growth and adaptation to the organization’s specific needs. These adaptations may include a unique design, polls and surveys, interfacing with external applications or adapting the interface to a range of devices ans apis.

## Prerequisites:
Since OpenideaL relies on [Drupal](https://www.drupal.org/) and is subject to its system requirements, it is recommended ensuring that you have satisfied the [Drupal System Requirements](https://www.drupal.org/docs/system-requirements) before moving forward.

## Build

OpenideaL is super easy to install. The following composer command will install the full codebase, together with all the required dependencies and libraries:

- Please use PHP 7.4, don't use PHP 8 or 8.1 it won't work.
- Recommended composer 2 - really faster and uses a lot less memory. (It has been tested with composer 1, 2.0 - 2.3.5)

```
composer create-project linnovate/openideal-composer openideal
```

Once the command has finishd executing, the `web` directory will hold all the necessary files to run OpenideaL. Proceed to installation of the site(s).

If you run into a `Fatal error: Allowed memory size of xxxxxxxx bytes exhausted`, try running the above command like this:

```
COMPOSER_MEMORY_LIMIT=-1 composer create-project linnovate/openideal-composer openideal
```


## Installation

The easiest installation method is using `drush`, which is available at the root folder of the build above. 

When at the `web` directory, run the following: 

`../vendor/bin/drush si -y --account-name username --account-pass my_pass --account-mail my_mail@example.com --site-name "OpenideaL" --db-url=mysql://dbuser@127.0.0.1/db_name idea`

- Change `username` with your username
- Change `my_pass` with your password
- Change `my_mail@example.com` with your email address
- Change `dbuser@127.0.0.1/db_name` with your DB settings
- The last bit - `idea` is the name of the profile

## Update

At any point you can run

`composer require linnovate/openideal:[version-number]` where `[version number]` is the desired tag.
`composer update`
`drush updb`
`drush cr`
`drush cim`

To upgrade Drupal core version run 

`composer update drupal/core 'drupal/core-*' --with-dependencies` , then run `drush updb`. 

## License

This project is licensed under the [GNU General Public License, version 2 or later](http://www.gnu.org/licenses/old-licenses/gpl-2.0.html). See [this page](https://www.drupal.org/about/licensing) on drupal.org for more details. 

