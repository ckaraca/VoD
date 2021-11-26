<?PHP
	require("../libs/vodlib.php");
	Authorize($hash,$user);
	if(!isset($hash)) header("location:error.php?errno=3");
	$link=DBconnect();
	$result = mysql_query("SELECT priv FROM user WHERE user='$user'", $link);
	if (!$result) die("query failed: ".mysql_error());
	$row = mysql_fetch_array($result);
	$priv = $row["priv"];
	DBdisconnect($link);
	switch($priv)
	{
		case 0: // student
		?>
		var NoOffFirstLineMenus=4;			// Number of main menu  items
		
		<?
		break;
		case 1: // Instr
		?>
		var NoOffFirstLineMenus=5;			// Number of main menu  items
		<?
		break;
		case 2:	// Admin
		?>
		var NoOffFirstLineMenus=6;			// Number of main menu  items
		<?
		break;
	}
?>
/***********************************************************************************
*	(c) VoD-2003                                                                   *
***********************************************************************************/
var LowBgColor="#666699";
var HighBgColor="#dddddd";
var FontLowColor="#dddddd";
var FontHighColor="#6633cc";
var BorderColor="";
var BorderWidthMain=5;
var BorderWidthSub=5;
var BorderBtwnMain=1;
var BorderBtwnSub=1;
var FontFamily="Tahoma";
var FontSize=10;
var FontBold=1;
var FontItalic=0;
var MenuTextCentered="center";


	var MenuCentered="center";			// Menu horizontal position can be: left, center, right, justify,
						//  leftjustify, centerjustify or rightjustify. PartOfWindow determines part of window to use
	var MenuVerticalCentered="top";		// Menu vertical position top, middle,bottom or static
	var ChildOverlap=.2;				// horizontal overlap child/ parent
	var ChildVerticalOverlap=.2;			// vertical overlap child/ parent
	var StartTop=50;				// Menu offset x coordinate
	var StartLeft=0;				// Menu offset y coordinate
	var VerCorrect=0;				// Multiple frames y correction
	var HorCorrect=0;				// Multiple frames x correction
	var LeftPaddng=3;				// Left padding
	var TopPaddng=2;				// Top padding
	var FirstLineHorizontal=1;			// First level items layout horizontal 1 or 0
	var MenuFramesVertical=0;			// Frames in cols or rows 1 or 0
	var DissapearDelay=300;			// delay before menu folds in
	var UnfoldDelay=150;			// delay before sub unfolds	
	var TakeOverBgColor=1;			// Menu frame takes over background color subitem frame
	var FirstLineFrame="header";			// Frame where first level appears
	var SecLineFrame="space";			// Frame where sub levels appear
	var DocTargetFrame="main";			// Frame where target documents appear
	var TargetLoc="";				// span id for relative positioning
	var MenuWrap=0;				// enables/ disables menu wrap 1 or 0
	var RightToLeft=0;				// enables/ disables right to left unfold 1 or 0
	var BottomUp=0;				// enables/ disables Bottom up unfold 1 or 0
	var UnfoldsOnClick=0;			// Level 1 unfolds onclick/ onmouseover
	var BaseHref="http://vod.emu.edu.tr/";				// BaseHref lets you specify the root directory for relative links. 
						// The script precedes your relative links with BaseHref
						// For instance: 
						// when your BaseHref= "http://www.MyDomain/" and a link in the menu is "subdir/MyFile.htm",
						// the script renders to: "http://www.MyDomain/subdir/MyFile.htm"
						// Can also be used when you use images in the textfields of the menu
						// "MenuX=new Array("<img src=\""+BaseHref+"MyImage\">"
						// For testing on your harddisk use syntax like: BaseHref="file:///C|/MyFiles/Homepage/"

	var Arrws=[BaseHref+"images/tri.gif",5,10,BaseHref+"tridown.gif",10,5,BaseHref+"trileft.gif",5,10,BaseHref+"triup.gif",10,5];

						// Arrow source, width and height.
						// If arrow images are not needed keep source ""

	var MenuUsesFrames=0;			// MenuUsesFrames is only 0 when Main menu, submenus,
						// document targets and script are in the same frame.
						// In all other cases it must be 1

	var RememberStatus=0;			// RememberStatus: When set to 1, menu unfolds to the presetted menu item. 
						// When set to 2 only the relevant main item stays highligthed
						// The preset is done by setting a variable in the head section of the target document.
						// <head>
						//	<script type="text/javascript">var SetMenu="2_2_1";</script>
						// </head>
						// 2_2_1 represents the menu item Menu2_2_1=new Array(.......

	var PartOfWindow=.8;			// PartOfWindow: When MenuCentered is justify, sets part of window width to stretch to

						// Below some pretty useless effects, since only IE6+ supports them
						// I provided 3 effects: MenuSlide, MenuShadow and MenuOpacity
						// If you don't need MenuSlide just leave in the line var MenuSlide="";
						// delete the other MenuSlide statements
						// In general leave the MenuSlide you need in and delete the others.
						// Above is also valid for MenuShadow and MenuOpacity
						// You can also use other effects by specifying another filter for MenuShadow and MenuOpacity.
						// You can add more filters by concanating the strings

var MenuSlide="progid:DXImageTransform.Microsoft.RevealTrans(duration=.4, transition=10)";
var MenuShadow="progid:DXImageTransform.Microsoft.Shadow(color=#333333, direction=135, strength=7)";
var MenuOpacity="progid:DXImageTransform.Microsoft.Alpha(opacity=90)";
var BuildOnDemand=0;


	function BeforeStart(){return}
	function AfterBuild(){return}
	function BeforeFirstOpen(){return}
	function AfterCloseAll(){return}

// Menu tree:
// MenuX=new Array("ItemText","Link","background image",number of sub elements,height,width,"bgcolor","bghighcolor",
//	"fontcolor","fonthighcolor","bordercolor","fontfamily",fontsize,fontbold,fontitalic,"textalign","statustext");
// Color and font variables defined in the menu tree take precedence over the global variables
// Fontsize, fontbold and fontitalic are ignored when set to -1.
// For rollover images ItemText format is:  "rollover?"+BaseHref+"Image1.jpg?"+BaseHref+"Image2.jpg" 
<?
switch($priv)
	{
		case 0: // student
		?>
		Menu1=new Array("Students","","",1,20,100,"#00CCCC","#0099FF","white","white","","",-1,1,-1,"","");	
		Menu1_1=new Array("Lectures","vod.php?action=list.php","",0,20,120,"#00CCCC","#0099FF","blue","white","","",10,1,-1,"","");
	
		Menu2=new Array("Forums","forum","",0,20,100,"#00CCCC","#0099FF","white","white","","",-1,1,-1,"","");
		Menu3=new Array("Logout","logout.php","",0,20,100,"#00CCCC","#0099FF","white","white","","",-1,1,-1,"","");
		Menu4=new Array("Home","index.php","",0,20,100,"#00CCCC","#0099FF","white","white","","",-1,1,-1,"","");
		<?
		break;
		case 1: // Instr
		?>
		Menu1=new Array("Students","","",1,20,100,"#00CCCC","#0099FF","white","white","","",-1,1,-1,"","");	
		Menu1_1=new Array("Lectures","vod.php?action=list.php","",0,20,120,"#00CCCC","#0099FF","blue","white","","",10,1,-1,"","");
	
		Menu2=new Array("Academics","","",1,20,100,"#00CCCC","#0099FF","white","white","","",-1,1,-1,"","");
		Menu2_1=new Array("Projects >","","",3,20,120,"#00CCCC","#0099FF","blue","white","","",10,1,-1,"","");
		Menu2_1_1=new Array("Add New Project","","",0,20,120,"#00CCCC","#0099FF","blue","white","","",10,1,-1,"","");
		Menu2_1_2=new Array("Modify Projects","","",0,20,120,"#00CCCC","#0099FF","blue","white","","",10,1,-1,"","");
		Menu2_1_3=new Array("Search Projects","","",0,20,120,"#00CCCC","#0099FF","blue","white","","",10,1,-1,"","");
		
		Menu3=new Array("Forums","forum","",0,20,100,"#00CCCC","#0099FF","white","white","","",-1,1,-1,"","");
		
		Menu4=new Array("Logout","logout.php","",0,20,100,"#00CCCC","#0099FF","white","white","","",-1,1,-1,"","");
		Menu5=new Array("Home","index.php","",0,20,100,"#00CCCC","#0099FF","white","white","","",-1,1,-1,"","");
		<?
		break;
		case 2:	// Admin
		?>
	Menu1=new Array("Students","","",1,20,100,"#00CCCC","#0099FF","white","white","","",-1,1,-1,"","");	
	Menu1_1=new Array("Lectures","vod.php?action=list.php","",0,20,120,"#00CCCC","#0099FF","blue","white","","",10,1,-1,"","");
	
Menu2=new Array("Academics","","",1,20,100,"#00CCCC","#0099FF","white","white","","",-1,1,-1,"","");
	Menu2_1=new Array("Projects >","","",3,20,120,"#00CCCC","#0099FF","blue","white","","",10,1,-1,"","");
		Menu2_1_1=new Array("Add New Project","","",0,20,120,"#00CCCC","#0099FF","blue","white","","",10,1,-1,"","");
		Menu2_1_2=new Array("Modify Projects","","",0,20,120,"#00CCCC","#0099FF","blue","white","","",10,1,-1,"","");
		Menu2_1_3=new Array("Search Projects","","",0,20,120,"#00CCCC","#0099FF","blue","white","","",10,1,-1,"","");
		
Menu3=new Array("Forums","forum","",0,20,100,"#00CCCC","#0099FF","white","white","","",-1,1,-1,"","");


Menu4=new Array("Administration","","",8,20,100,"#00CCCC","#0099FF","white","white","","",-1,1,-1,"","");
	Menu4_1=new Array("System Settings","vod.php?action=adm/system.php","",0,20,120,"#00CCCC","#0099FF","blue","white","","",10,1,-1,"","");
	Menu4_2=new Array("User Operations >","","",4,20,180,"#00CCCC","#0099FF","blue","white","","",10,1,-1,"","");
		Menu4_2_1=new Array("User list","vod.php?action=adm/userlist.php&op=1","",0,20,180,"#00CCCC","#0099FF","blue","white","","",10,1,-1,"","");
		Menu4_2_2=new Array("Add new user","vod.php?action=adm/newuser.php","",0,20,180,"#00CCCC","#0099FF","blue","white","","",10,1,-1,"","");
		Menu4_2_3=new Array("Modify Users","vod.php?action=adm/userlist.php&op=2","",0,20,180,"#00CCCC","#0099FF","blue","white","","",10,1,-1,"","");
		Menu4_2_4=new Array("Delete users","vod.php?action=adm/userlist.php&op=3","",0,20,180,"#00CCCC","#0099FF","blue","white","","",10,1,-1,"","");
	Menu4_3=new Array("Faculties >","","",2,20,180,"#00CCCC","#0099FF","blue","white","","",10,1,-1,"","");
		Menu4_3_1=new Array("Add new faculty","","",0,20,180,"#00CCCC","#0099FF","blue","white","","",10,1,-1,"","");
		Menu4_3_2=new Array("Modify/Delete faculty","vod.php?action=adm/facultylist.php","",0,20,180,"#00CCCC","#0099FF","blue","white","","",10,1,-1,"","");
	Menu4_4=new Array("Departments >","","",2,20,180,"#00CCCC","#0099FF","blue","white","","",10,1,-1,"","");
		Menu4_4_1=new Array("Add new department","","",0,20,180,"#00CCCC","#0099FF","blue","white","","",10,1,-1,"","");
		Menu4_4_2=new Array("Modify/Delete departments","vod.php?action=adm/departmentlist.php","",0,20,180,"#00CCCC","#0099FF","blue","white","","",10,1,-1,"","");
	Menu4_5=new Array("Courses >","","",2,20,180,"#00CCCC","#0099FF","blue","white","","",10,1,-1,"","");
		Menu4_5_1=new Array("Add new course","","",0,20,180,"#00CCCC","#0099FF","blue","white","","",10,1,-1,"","");
		Menu4_5_2=new Array("Modify/Delete course","","",0,20,180,"#00CCCC","#0099FF","blue","white","","",10,1,-1,"","");
	Menu4_6=new Array("Chapters >","","",2,20,180,"#00CCCC","#0099FF","blue","white","","",10,1,-1,"","");
		Menu4_6_1=new Array("Add new chapter","","",0,20,180,"#00CCCC","#0099FF","blue","white","","",10,1,-1,"","");
		Menu4_6_2=new Array("Modify/Delete chapter","","",0,20,180,"#00CCCC","#0099FF","blue","white","","",10,1,-1,"","");
	Menu4_7=new Array("Content Servers","","",4,20,200,"#00CCCC","#0099FF","blue","white","","",10,1,-1,"","");
		Menu4_7_1=new Array("Server list","vod.php?action=adm/serverlist.php&op=1","",0,20,180,"#00CCCC","#0099FF","blue","white","","",10,1,-1,"","");
		Menu4_7_2=new Array("Add new server","vod.php?action=adm/newserver.php","",0,20,180,"#00CCCC","#0099FF","blue","white","","",10,1,-1,"","");
		Menu4_7_3=new Array("Modify servers","vod.php?action=adm/serverlist.php&op=2","",0,20,180,"#00CCCC","#0099FF","blue","white","","",10,1,-1,"","");
		Menu4_7_4=new Array("Delete servers","vod.php?action=adm/serverlist.php&op=3","",0,20,180,"#00CCCC","#0099FF","blue","white","","",10,1,-1,"","");
	Menu4_8=new Array("Statistics","","",0,20,180,"#00CCCC","#0099FF","blue","white","","",10,1,-1,"","");
	Menu5=new Array("Logout","logout.php","",0,20,100,"#00CCCC","#0099FF","white","white","","",-1,1,-1,"","");
	Menu6=new Array("Home","index.php","",0,20,100,"#00CCCC","#0099FF","white","white","","",-1,1,-1,"","");


		
		<?
		break;
	}
	?>

