<?php
/**
 * @package WordPress
 * @subpackage GoPress Theme
 */
$options = get_option( 'gopress_theme_settings' );
?>
<!DOCTYPE html>

<!-- BEGIN html -->
<html <?php language_attributes(); ?>>

<!-- BEGIN head -->
<head>

<!-- Meta Tags -->
<meta http-equiv="Content-Type" content="<?php bloginfo('html_type'); ?>; charset=<?php bloginfo('charset'); ?>" />

<title><?php wp_title(''); ?><?php if(wp_title('', false)) { echo ' |'; } ?> <?php bloginfo('name'); ?></title>
    
<!-- Stylesheet & Favicon -->
<?php if($options['favicon'] !='') { ?>
<link rel="icon" type="image/png" href="<?php echo $options['favicon']; ?>" />
<?php } ?>
<link rel="stylesheet" type="text/css" href="<?php bloginfo('stylesheet_url'); ?>" />
<link rel="stylesheet" type="text/css" href="<?php bloginfo('template_directory'); ?>/style-cec.css" />

<!-- WP Head -->
<?php if ( is_single() || is_page() ) wp_enqueue_script( 'comment-reply' ); ?>
<?php wp_head(); ?>

</head>
<body <?php body_class(); ?>>

<div id="wrap" class="clearfix">

<div class="superMenu">
  <ul>
  <?php $menu = $_GET["menu"]; ?>
  <li<?php if (!isset($menu)) {echo ' id="actif">'; $end='';} else{echo '><a href="/index.php">'; $end='</a>';} ?><span>Accueil</span><?php echo $end ?></li>
  <li<?php if ($menu=='recettes') {echo ' id="actif">'; $end='';} else{echo '><a href="/recette_category/recettes/?menu=recettes">'; $end='</a>';} ?><span>RECETTES</span><?php echo $end ?></li>
  <li<?php if ($menu=='conseils') {echo ' id="actif">'; $end='';} else{echo '><a href="/recette_category/conseils?menu=conseils">'; $end='</a>';} ?><span>CONSEILS</span><?php echo $end ?></li>
  <li<?php if ($menu=='contact') {echo ' id="actif">'; $end='';} else{echo '><a href="/contact?menu=contact">'; $end='</a>';} ?><span>Contact</span><?php echo $end ?></li>
  <li<?php if ($menu=='aPropos') {echo ' id="actif">'; $end='';} else{echo '><a href="/a-propos?menu=aPropos">'; $end='</a>';} ?><span>À propos</span><?php echo $end ?></li>
 </ul>
</div>

<div id="header" class="clearfix">

	<div id="header-top" class="clearfix">
        <div id="logo" style="margin-top: <?php if(!empty($options['logo_margin'])) { echo $options['logo_margin']; } else { echo '0'; } ?>px;">
        	<?php
            	if(!empty($options['upload_mainlogo'])) { ?>
					<a href="<?php echo home_url(); ?>/" title="<?php bloginfo( 'name' ); ?>"><img src="<?php echo $options['upload_mainlogo']; ?>" alt="<?php bloginfo( 'name' ) ?>" /></a>
			 	<?php } else { ?>
            	<a href="<?php echo home_url(); ?>/" title="<?php bloginfo( 'name' ); ?>"><?php bloginfo( 'name' ) ?></a>
            <?php } ?>          
        </div>
        <!-- END logo -->
        
        <?php if(!empty($options['header_banner'])) { ?>
        	<div id="header-banner"><?php echo $options['header_banner']; ?></div>
        <?php } ?> 
        
	</div>
       
	<?php
    $side_description = get_bloginfo( 'description' );
    if(!empty($side_description)) { echo '<div id="site-description">'. $side_description.'</div>'; }
    ?>
        
</div><!-- END header -->

        <div id="navigation" class="clearfix">
            <?php
            //define main navigation
            wp_nav_menu( array(
                'theme_location' => 'menu',
                'sort_column' => 'menu_order',
                'menu_class' => 'sf-menu',
                'fallback_cb' => 'default_menu'
            )); ?>
        </div>
        <!-- END navigation -->  
