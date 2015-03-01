<?php
/**
 * @package WordPress
 * @subpackage GoPress Theme
 */
//Pour crawlProtect :
require_once("/home/conseil3/public_html/crawlprotect/include/cppf.php");
//Reste du code (original) :
$options = get_option( 'gopress_theme_settings' );
?>
<?php get_header(); ?>
<?php if ( function_exists( 'get_wp_parallax_content_slider' ) ) { get_wp_parallax_content_slider(); } ?>
<?php if (have_posts()) : ?>

<div id="index" class="post clearfix">

	<?php
	$paged = $wp_query->get( 'paged' );
    if(!$paged) { require( TEMPLATEPATH . '/includes/slides.php'); } ?>
        
	<?php get_template_part( 'loop', 'entry') ?>
</div>

<?php get_sidebar(); ?>
<?php pagination(); ?>
<?php endif; ?>
<?php get_footer(); ?>
