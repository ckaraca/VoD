 /***********************************************************************************
*	(c) VoD-2003                                                                   *
***********************************************************************************/
	var NoOffFirstLineMenus=5;			// Number of main menu  items
						// Colorvariables:
						// Color variables take HTML predefined color names or "#rrggbb" strings
						//For transparency make colors and border color ""
	var LowBgColor="";			// Background color when mouse is not over
	var HighBgColor="";			// Background color when mouse is over
	var FontLowColor="";			// Font color when mouse is not over
	var FontHighColor="";			// Font color when mouse is over
	var BorderColor="";			// Border color
	var BorderWidth=1;				// Border width
	var BorderBtwnElmnts=1;			// Border between elements 1 or 0
	var FontFamily="arial"	// Font family menu items
	var FontSize=9;				// Font size menu items
	var FontBold=0;				// Bold menu items 1 or 0
	var FontItalic=0;				// Italic menu items 1 or 0
	var MenuTextCentered="center";		// Item text position left, center or right
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
	var BaseHref="";				// BaseHref lets you specify the root directory for relative links. 
						// The script precedes your relative links with BaseHref
						// For instance: 
						// when your BaseHref= "http://www.MyDomain/" and a link in the menu is "subdir/MyFile.htm",
						// the script renders to: "http://www.MyDomain/subdir/MyFile.htm"
						// Can also be used when you use images in the textfields of the menu
						// "MenuX=new Array("<img src=\""+BaseHref+"MyImage\">"
						// For testing on your harddisk use syntax like: BaseHref="file:///C|/MyFiles/Homepage/"

	var Arrws=[BaseHref+"tri.gif",5,10,BaseHref+"tridown.gif",10,5,BaseHref+"trileft.gif",5,10,BaseHref+"triup.gif",10,5];

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
	var MenuSlide="";
	var MenuSlide="progid:DXImageTransform.Microsoft.RevealTrans(duration=.5, transition=19)";
	var MenuSlide="progid:DXImageTransform.Microsoft.GradientWipe(duration=.0, wipeStyle=1)";

	var MenuShadow="";
	var MenuShadow="progid:DXImageTransform.Microsoft.DropShadow(color=#888888, offX=2, offY=2, positive=1)";
	var MenuShadow="progid:DXImageTransform.Microsoft.Shadow(color=#888888, direction=135, strength=0)";

	var MenuOpacity="";
	var MenuOpacity="progid:DXImageTransform.Microsoft.Alpha(opacity=100)";

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

Menu1=new Array("Students","","",1,20,100,"#00CCCC","#0099FF","white","white","","",-1,1,-1,"","");	
	Menu1_1=new Array("Lectures","vod.php?action=list.php","",0,20,120,"#00CCCC","#0099FF","blue","white","","",10,1,-1,"","");
	
Menu2=new Array("Academics","","",1,20,100,"#00CCCC","#0099FF","white","white","","",-1,1,-1,"","");
	Menu2_1=new Array("Projects >","","",3,20,120,"#00CCCC","#0099FF","blue","white","","",10,1,-1,"","");
		Menu2_1_1=new Array("Add New Project","","",0,20,120,"#00CCCC","#0099FF","blue","white","","",10,1,-1,"","");
		Menu2_1_2=new Array("Modify Projects","","",0,20,120,"#00CCCC","#0099FF","blue","white","","",10,1,-1,"","");
		Menu2_1_3=new Array("Search Projects","","",0,20,120,"#00CCCC","#0099FF","blue","white","","",10,1,-1,"","");
		
Menu3=new Array("Forums","","",0,20,100,"#00CCCC","#0099FF","white","white","","",-1,1,-1,"","");


Menu4=new Array("Administration","","",8,20,100,"#00CCCC","#0099FF","white","white","","",-1,1,-1,"","");
	Menu4_1=new Array("System Settings","vod.php?action=admin/system.php","",0,20,120,"#00CCCC","#0099FF","blue","white","","",10,1,-1,"","");
	Menu4_2=new Array("User Operations >","","",4,20,180,"#00CCCC","#0099FF","blue","white","yellow","",10,1,-1,"","");
		Menu4_2_1=new Array("User list","vod.php?action=admin/userlist.php&op=1","",0,20,180,"#00CCCC","#0099FF","blue","white","yellow","",10,1,-1,"","");
		Menu4_2_2=new Array("Add new user","vod.php?action=admin/newuser.php","",0,20,180,"#00CCCC","#0099FF","blue","white","yellow","",10,1,-1,"","");
		Menu4_2_3=new Array("Modify Users","vod.php?action=admin/userlist.php&op=2","",0,20,180,"#00CCCC","#0099FF","blue","white","yellow","",10,1,-1,"","");
		Menu4_2_4=new Array("Delete users","vod.php?action=admin/userlist.php&op=3","",0,20,180,"#00CCCC","#0099FF","blue","white","yellow","",10,1,-1,"","");
	Menu4_3=new Array("Faculties >","","",2,20,180,"#00CCCC","#0099FF","blue","white","yellow","",10,1,-1,"","");
		Menu4_3_1=new Array("Add new faculty","","",0,20,180,"#00CCCC","#0099FF","blue","white","yellow","",10,1,-1,"","");
		Menu4_3_2=new Array("Modify/Delete faculty","vod.php?action=admin/facultylist.php","",0,20,180,"#00CCCC","#0099FF","blue","white","yellow","",10,1,-1,"","");
	Menu4_4=new Array("Departments >","","",2,20,180,"#00CCCC","#0099FF","blue","white","yellow","",10,1,-1,"","");
		Menu4_4_1=new Array("Add new department","","",0,20,180,"#00CCCC","#0099FF","blue","white","yellow","",10,1,-1,"","");
		Menu4_4_2=new Array("Modify/Delete departments","vod.php?action=admin/departmentlist.php","",0,20,180,"#00CCCC","#0099FF","blue","white","yellow","",10,1,-1,"","");
	Menu4_5=new Array("Courses >","","",2,20,180,"#00CCCC","#0099FF","blue","white","yellow","",10,1,-1,"","");
		Menu4_5_1=new Array("Add new course","","",0,20,180,"#00CCCC","#0099FF","blue","white","yellow","",10,1,-1,"","");
		Menu4_5_2=new Array("Modify/Delete course","","",0,20,180,"#00CCCC","#0099FF","blue","white","yellow","",10,1,-1,"","");
	Menu4_6=new Array("Chapters >","","",2,20,180,"#00CCCC","#0099FF","blue","white","yellow","",10,1,-1,"","");
		Menu4_6_1=new Array("Add new chapter","","",0,20,180,"#00CCCC","#0099FF","blue","white","yellow","",10,1,-1,"","");
		Menu4_6_2=new Array("Modify/Delete chapter","","",0,20,180,"#00CCCC","#0099FF","blue","white","yellow","",10,1,-1,"","");
	Menu4_7=new Array("FTP Settings","","",0,20,180,"#00CCCC","#0099FF","blue","white","yellow","",10,1,-1,"","");
	Menu4_8=new Array("Statistics","","",0,20,180,"#00CCCC","#0099FF","blue","white","yellow","",10,1,-1,"","");

Menu5=new Array("Logout","logout.php","",0,20,100,"#00CCCC","#0099FF","white","white","","",-1,1,-1,"","");
