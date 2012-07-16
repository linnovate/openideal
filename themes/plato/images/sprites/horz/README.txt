##### Sasson - advanced drupal theming. #####

Here you can place you images for a horizontal sprite image.
To generate the sprite you'll have to call it from your stylesheet, e.g. :

  @import "sprites/horz";

then you will be able to use auto-generated mixins
and apply these images as background images, e.g. :

  #selector {
    @include sprite-horz-IMAGENAME;
  }

Every directory under /sprites will become a seperate sprite image, you can
create as many directories (or sprite images) as you want. keep in mind that
'horz' in the directory name will generate a horizontal sprite, 'vert' will
generate a vertical sprite and anything else will generate a smart sprite.
