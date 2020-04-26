FROM php:5.6-apache

# Install dependency packages
WORKDIR /
RUN apt update
RUN apt upgrade -y
RUN apt install -y git zip unzip libpng-dev zlib1g-dev libzip-dev

# Enable php extensions
RUN docker-php-ext-install mysqli
RUN docker-php-ext-install pdo_mysql
RUN docker-php-ext-install mbstring
RUN docker-php-ext-install zip
RUN docker-php-ext-install gd

# Install composer and drush
RUN curl -sS https://getcomposer.org/installer | php
RUN mv composer.phar /usr/local/bin/composer
RUN composer require drush/drush:8.x

# Install openideal
COPY . /openideal
RUN /vendor/bin/drush make /openideal/build-openideal-github.make /var/www/html/openideal
WORKDIR /var/www/html
RUN chown -R www-data:www-data .
