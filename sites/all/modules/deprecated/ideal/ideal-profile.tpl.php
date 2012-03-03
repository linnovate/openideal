<?php
	global $user;
?>
<?php if ($user->uid != 0):?>
	<?php print theme('user_picture', $user);?>
	<?php print t('hello, !username', array('!username' => theme('username', $user)));?>
	<?php print '<br/>'.l(t('logout'), 'logout');?>
<?php else:?>
	<?php print '<h3>'.t('login using facebook').'</h3>';?>
	<?php print fbconnect_render_button();?>
<?endif;?>