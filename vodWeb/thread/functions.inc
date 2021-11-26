<?php
function getUriReplyForum ( $theforum_id, $theparent_id )
{
	echo "reply.php?forum_id=";
	echo $theforum_id;
	echo "&Parent=";
	echo $theparent_id;
}

function getUriNew( $theforum_id )
{
	echo "new.php?forum_id=";
	echo $theforum_id;
}

function getUriSelectMsg( $item_identifier )
{
global $forum_id;
echo "index.php?";
echo "forum_id=";
echo $forum_id;
echo "&select=";
echo $item_identifier;
}

// draw the indent graphics.
// this function draws the indent and the article-image, based on the
// indent-level and the state of the article.

function WriteIndent( $ind_lvl, $state )
{
	global $ARTICLE_INDENT, $ARTICLE_TITLE_HEIGHT;
	$i = 1;
	for ($i=1; $i<$ind_lvl; $i++)
	{
		echo "<img border=\"0\" height=\"1\" width=\"";
		echo $ARTICLE_INDENT;
		echo "\" src=\"img_forum/ind.gif\">";
	}

	if ($state=="selected")
		echo "<img border=\"0\" align=\"absmiddle\" src=\"img_forum/news_selected.gif\">";	// use news_new.gif
	else if ($state=="content")
		echo "<img border=\"0\" align=\"absmiddle\" src=\"img_forum/news_none.gif\">";
	else
		echo "<img border=\"0\" align=\"absmiddle\" src=\"img_forum/news_unselected.gif\">"; // use news_new.gif

	echo "<img border=\"0\" width=\"2\" height=\"".$ARTICLE_TITLE_HEIGHT."\" align=\"absmiddle\" src=\"img_forum/t.gif\">";
}

function format_output($output) {
/****************************************************************************
 * Takes a raw string ($output) and formats it for output using a special
 * stripped down markup that is similar to HTML
 ****************************************************************************/

	$output = htmlspecialchars(stripslashes($output));

	// new paragraph
//	$output = str_replace('[p]', '<p>', $output);

	// bold
//	$output = str_replace('[b]', '<b>', $output);
//	$output = str_replace('[/b]', '</b>', $output);

	// italics
//	$output = str_replace('[i]', '<i>', $output);
//	$output = str_replace('[/i]', '</i>', $output);

	// preformatted
//	$output = str_replace('[pre]', '<pre>', $output);
//	$output = str_replace('[/pre]', '</pre>', $output);

	// indented blocks (blockquote)
//	$output = str_replace('[indent]', '<blockquote>', $output);
//	$output = str_replace('[/indent]', '</blockquote>', $output);

	// anchors
//	$output = ereg_replace('\[anchor=&quot;([[:graph:]]+)&quot;\]', '<a name="\\1"></a>', $output);
	
	// links, note we try to prevent javascript in links
//	$output = str_replace('[link=&quot;javascript', '[link=&quot; javascript', $output);
//	$output = ereg_replace('\[link=&quot;([[:graph:]]+)&quot;\]', '<a href="\\1">', $output);
//	$output = str_replace('[/link]', '</a>', $output);	
	
	return nl2br($output);
}

// helper function to format the content of an article.
// replaces several smilies by graphical representations.
// NOTE: these images were "lend" from the lionhead forum
//       (www.lionhead.co.uk), use at your own risk!
function fmtContent( $text )
{
//	text = fmtFilter(text)	' Filter out malcious stuff
//	text = fmtML(text)	' Format line breaks

	$text = format_output($text);
	$text = str_replace( ":-)"        , "<img src=\"img_forum/smiley_smile.gif\" align=\"absmiddle\" border=\"0\">", $text );
	$text = str_replace( ":)"         , "<img src=\"img_forum/smiley_smile.gif\" align=\"absmiddle\" border=\"0\">", $text );

	$text = str_replace( ":-("        , "<img src=\"img_forum/smiley_frown.gif\" align=\"absmiddle\" border=\"0\">", $text );
	$text = str_replace( ":("        , "<img src=\"img_forum/smiley_frown.gif\" align=\"absmiddle\" border=\"0\">", $text );

	$text = str_replace( ":-O"        , "<img src=\"img_forum/smiley_redface.gif\" align=\"absmiddle\" border=\"0\">", $text );
	$text = str_replace( ":-o"        , "<img src=\"img_forum/smiley_redface.gif\" align=\"absmiddle\" border=\"0\">", $text );

	$text = str_replace( ":-0"        , "<img src=\"img_forum/smiley_redface.gif\" align=\"absmiddle\" border=\"0\">", $text );
	$text = str_replace( ":O "        , "<img src=\"img_forum/smiley_redface.gif\" align=\"absmiddle\" border=\"0\">", $text );
	$text = str_replace( ":o "        , "<img src=\"img_forum/smiley_redface.gif\" align=\"absmiddle\" border=\"0\">", $text );
	$text = str_replace( ":0 "        , "<img src=\"img_forum/smiley_redface.gif\" align=\"absmiddle\" border=\"0\">", $text );

	$text = str_replace( ":-D "        , "<img src=\"img_forum/smiley_biggrin.gif\" align=\"absmiddle\" border=\"0\">", $text );
	$text = str_replace( ":D "        , "<img src=\"img_forum/smiley_biggrin.gif\" align=\"absmiddle\" border=\"0\">", $text );

	$text = str_replace( ";-)"        , "<img src=\"img_forum/smiley_wink.gif\" align=\"absmiddle\" border=\"0\">", $text );
	$text = str_replace( ";)"        , "<img src=\"img_forum/smiley_wink.gif\" align=\"absmiddle\" border=\"0\">", $text );

	$text = str_replace( ";-p"        , "<img src=\"img_forum/smiley_tongue.gif\" align=\"absmiddle\" border=\"0\">", $text );
	$text = str_replace( ";-P"        , "<img src=\"img_forum/smiley_tongue.gif\" align=\"absmiddle\" border=\"0\">", $text );
	$text = str_replace( ";p"        , "<img src=\"img_forum/smiley_tongue.gif\" align=\"absmiddle\" border=\"0\">", $text );
	$text = str_replace( ";P"        , "<img src=\"img_forum/smiley_tongue.gif\" align=\"absmiddle\" border=\"0\">", $text );
	$text = str_replace( ":P"        , "<img src=\"img_forum/smiley_tongue.gif\" align=\"absmiddle\" border=\"0\">", $text );
	$text = str_replace( ":p"        , "<img src=\"img_forum/smiley_tongue.gif\" align=\"absmiddle\" border=\"0\">", $text );

	$text = str_replace( ":cool:"        , "<img src=\"img_forum/smiley_cool.gif\" align=\"absmiddle\" border=\"0\">", $text );

	$text = str_replace( ":rolleyes:"        , "<img src=\"img_forum/smiley_rolleyes.gif\" align=\"absmiddle\" border=\"0\">", $text );
	$text = str_replace( ":mad:"        , "<img src=\"img_forum/smiley_mad.gif\" align=\"absmiddle\" border=\"0\">", $text );
	$text = str_replace( ":eek:"        , "<img src=\"img_forum/smiley_eek.gif\" align=\"absmiddle\" border=\"0\">", $text );
	$text = str_replace( ":confused:"        , "<img src=\"img_forum/smiley_confused.gif\" align=\"absmiddle\" border=\"0\">", $text );
	return $text;
}
?>