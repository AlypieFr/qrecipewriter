<?php
/**
 * @package WordPress
 * @subpackage GoPress Theme
 */
?>
<?php get_header(' '); ?>

		<?php
        $paged = (get_query_var('paged')) ? get_query_var('paged') : 1;
#		query_posts($query_string .'&posts_per_page=10&paged=' . $paged);
		if (have_posts()) : ?>
        
        	 <div id="page-heading">
				<h1 id="archive-title">Résultats de la recherche pour : <?php the_search_query(); ?></h1>
				<?php 
				if(isset($_GET["catSelect"])) {
					echo "Catégories recherchées : ".$_GET["catSelect"].".<br>";
				}
				if(isset($_GET["tpsTotalMax"])) {
					echo "Temps nécessaire maximum : ".$_GET["tpsTotalMax"]."<br>";
				}
				if(isset($_GET["tpsPrepMax"])) {
					echo "Temps de préparation maximum : ".$_GET["tpsPrepMax"]."<br>";
				}
				if(isset($_GET["tpsCuisMax"])) {
					echo "Temps de cuisson maximum : ".$_GET["tpsCuisMax"];
				}
				?>
            </div>
			<!-- END page-heading -->
            
			<div id="post" class="post clearfix">
            
            	<div id="archive">

				<?php get_template_part( 'loop' , 'entry') ?>
                
                </div>
        
        		<?php pagination();?>
        
			</div>
			<!-- END post  -->
        
		<?php else : ?>
        
			<div id="page-heading">
            <h1 id="archive-title">Résutats de la recherche pour : "<?php the_search_query(); ?>"</h1>
        	</div>
            <!-- END page-heading -->
            
            <div id="post" class="post clearfix">
            <?php _e('Aucune recette trouvée.', 'gopress'); ?>
			</div>
			<!-- END post  -->
            
        <?php endif; ?>

<?php get_sidebar(' '); ?>		  
<?php get_footer(' '); ?>
