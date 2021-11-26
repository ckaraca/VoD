<?php
/* $Id: image_png__inline.inc.php,v 1.2 2003/07/07 12:06:50 garvinhicking Exp $ */
// vim: expandtab sw=4 ts=4 sts=4:

if (!defined('PMA_TRANSFORMATION_IMAGE_PNG__INLINE')){
    define('PMA_TRANSFORMATION_IMAGE_PNG__INLINE', 1);

    function PMA_transformation_image_png__inline($buffer, $options = array()) {
        include('./libraries/transformations/global.inc.php');
        
        if (PMA_IS_GD2) {
            $transform_options = array ('string' => '<a href="transformation_wrapper.php' . $options['wrapper_link'] . '" target="_blank"><img src="transformation_wrapper.php' . $options['wrapper_link'] . '&amp;resize=png&amp;newWidth=' . (isset($options[0]) ? $options[0] : '100') . '&amp;newHeight=' . (isset($options[1]) ? $options[1] : 100) . '" alt="[__BUFFER__]" border="0"></a>');
        } else {
            $transform_options = array ('string' => '<img src="transformation_wrapper.php' . $options['wrapper_link'] . '" alt="[__BUFFER__]" width="320" height="240">');
        }
        $buffer = PMA_transformation_global_html_replace($buffer, $transform_options);
        
        return $buffer;
    }
}
