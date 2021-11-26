<?php

if ( phpversion() >= "4.2.0")
   	{
		extract($_POST);
   		extract($_GET);
   		extract($_SERVER);
	   	extract($_FILES);
   		extract($_ENV);
   		extract($_COOKIE);
   	}

	$COLOR_SELECTED_BG	= "#53B398";	// background color of the headline of a selected article.
	$COLOR_SELECTED_FG	= "#000000";	// fontcolor of the headline of a selected article.

	$COLOR_CONTENT_BG	= "#B7DFD5";	// background color of the content(body) of a selected article.
	$COLOR_CONTENT_FG	= "#0000FF";	//fontcolor of the content(body) of a selected article.

	$COLOR_NONSELECTED_BG	= "#EDF8F4";	// background color of the headline of a not-selected article.
	$COLOR_NONSELECTED_FG	= "#000000";	// foreground color of the headline of a not-selected article.

	$COLOR_HEADER_BG	= "#B7DFD5";	// background color of the headline "subject-author-date"

	$COLOR_REPLY_BG		= "#00896E";	// background color of the reply-header
	$COLOR_REPLY_FONT	= "#FFFFFF";	// font color of the reply-header

	$FONT_REPLY_FORM1 = "<font face=\"Verdana, Arial, Helvetica, sans-serif\" size=\"1\" color=\"#000000\">";
	$FONT_REPLY_FORM2 = "<font face=\"Verdana, Arial, Helvetica, sans-serif\" size=\"2\" color=\"#000000\">";
	$FONT_REPLY_FORM3 = "<font face=\"Verdana, Arial, Helvetica, sans-serif\" size=\"3\" color=\"#000000\">";
	$FONT_REPLY_FORM4 = "<font face=\"Verdana, Arial, Helvetica, sans-serif\" size=\"4\" color=\"#000000\">";

	$FONT1 = "<font face=\"Verdana, Arial, Helvetica, sans-serif\" size=\"1\" color=\"#000000\">";
	$FONT2 = "<font face=\"Verdana, Arial, Helvetica, sans-serif\" size=\"2\" color=\"#000000\">";
	$FONT3 = "<font face=\"Verdana, Arial, Helvetica, sans-serif\" size=\"3\" color=\"#000000\">";
	$FONT4 = "<font face=\"Verdana, Arial, Helvetica, sans-serif\" size=\"4\" color=\"#000000\">";

	$hn='localhost';
	$un='control';
	$pw='ankara';
	$db='VoD';

	$ARTICLE_SEPERATION = 4;	// Vertical seperation between articles
	$ARTICLE_TITLE_HEIGHT = 15;	// Height of article title
	$ARTICLE_INDENT = 15;		// Horizontal indentation of articles

?>