# phpMyAdmin MySQL-Dump
# version 2.4.0
# http://www.phpmyadmin.net/ (download page)
#
# Sunucu:: localhost
# Çýktý Tarihi: Aralýk 11, 2003 at 06:28 AM
# Server sürümü: 4.0.13
# PHP Sürümü: 4.3.1
# Veritabaný : `vod`
# --------------------------------------------------------

#
# Tablo için tablo yapýsý `chapter`
#

CREATE TABLE chapter (
  id int(1) unsigned NOT NULL auto_increment,
  name varchar(100) default NULL,
  csid int(1) unsigned NOT NULL default '0',
  chapterdesc text,
  PRIMARY KEY  (id)
) TYPE=MyISAM;

#
# Tablo döküm verisi `chapter`
#

INSERT INTO chapter VALUES (14, 'Star sailor', 4, '');
INSERT INTO chapter VALUES (12, 'Queen', 4, '');
INSERT INTO chapter VALUES (11, 'Madonna', 4, '');
INSERT INTO chapter VALUES (5, 'Chapter 5', 1, 'c');
INSERT INTO chapter VALUES (6, 'ch 1', 2, 'd');
INSERT INTO chapter VALUES (7, 'Chapter 1', 3, 'EEE Dept. Model Animations');
INSERT INTO chapter VALUES (8, 'Twain', 4, 'e');
INSERT INTO chapter VALUES (10, 'enya', 4, '');
INSERT INTO chapter VALUES (13, 'Chapter 1', 5, '');
INSERT INTO chapter VALUES (15, 'Nirvana', 6, '');
INSERT INTO chapter VALUES (16, 'Judas', 6, '');
INSERT INTO chapter VALUES (17, 'P.O.D', 6, '');
INSERT INTO chapter VALUES (18, 'Yngwie Malmsteen', 6, '');
INSERT INTO chapter VALUES (19, 'EVANESCENCE', 4, '');
INSERT INTO chapter VALUES (20, 'cher', 4, '');
INSERT INTO chapter VALUES (21, 'THG', 1, '');
INSERT INTO chapter VALUES (22, 'FTP Tests', 1, '');
# --------------------------------------------------------

#
# Tablo için tablo yapýsý `course`
#

CREATE TABLE course (
  id int(1) unsigned NOT NULL auto_increment,
  depid int(1) unsigned NOT NULL default '0',
  cscode varchar(10) default NULL,
  name varchar(50) default NULL,
  instructor varchar(60) default NULL,
  nofst int(1) unsigned default NULL,
  coursedesc text,
  PRIMARY KEY  (id)
) TYPE=MyISAM;

#
# Tablo döküm verisi `course`
#

INSERT INTO course VALUES (1, 1, 'ee 500', 'Thesis', 'Derviþ Z. Deniz', NULL, '');
INSERT INTO course VALUES (2, 18, 'med 101', '', '', NULL, '');
INSERT INTO course VALUES (3, 1, 'EEE 474', 'Software Engineering', 'Dervis Z. Deniz', NULL, '');
INSERT INTO course VALUES (4, 10, 'Pop Music', 'Pop Video', '', NULL, '');
INSERT INTO course VALUES (5, 1, 'EE 511', 'Design, Analysis and Performance of computer netwo', 'Dervis Z. Deniz', NULL, '');
INSERT INTO course VALUES (6, 10, 'Rock Music', 'MP3', '', NULL, '');
# --------------------------------------------------------

#
# Tablo için tablo yapýsý `department`
#

CREATE TABLE department (
  id int(1) unsigned NOT NULL auto_increment,
  facid int(1) unsigned NOT NULL default '0',
  name varchar(50) default NULL,
  PRIMARY KEY  (id)
) TYPE=MyISAM;

#
# Tablo döküm verisi `department`
#

INSERT INTO department VALUES (1, 1, 'Electrical & Electronic');
INSERT INTO department VALUES (2, 2, 'Architecture');
INSERT INTO department VALUES (3, 2, 'Interior Architecture');
INSERT INTO department VALUES (4, 3, 'Applied Mathematics & Computer Science');
INSERT INTO department VALUES (5, 3, 'Archaeology & History of Art');
INSERT INTO department VALUES (6, 3, 'Chemistry');
INSERT INTO department VALUES (7, 3, 'English Literature and Humanities');
INSERT INTO department VALUES (8, 3, 'History');
INSERT INTO department VALUES (9, 3, 'Information Systems');
INSERT INTO department VALUES (10, 3, 'Music');
INSERT INTO department VALUES (11, 3, 'Physics');
INSERT INTO department VALUES (12, 3, 'Turkish Language & Literature');
INSERT INTO department VALUES (13, 4, 'Banking & Finance');
INSERT INTO department VALUES (14, 4, 'Business Administration');
INSERT INTO department VALUES (15, 4, 'Economics');
INSERT INTO department VALUES (16, 4, 'International Relations');
INSERT INTO department VALUES (17, 4, 'Political Science & Public Administration');
INSERT INTO department VALUES (18, 8, 'Medicine');
# --------------------------------------------------------

#
# Tablo için tablo yapýsý `faculty`
#

CREATE TABLE faculty (
  id int(1) unsigned NOT NULL auto_increment,
  name varchar(50) default NULL,
  PRIMARY KEY  (id)
) TYPE=MyISAM;

#
# Tablo döküm verisi `faculty`
#

INSERT INTO faculty VALUES (1, 'Engineering');
INSERT INTO faculty VALUES (2, 'Architecture');
INSERT INTO faculty VALUES (3, 'Arts & Science');
INSERT INTO faculty VALUES (4, 'Business & Economics');
INSERT INTO faculty VALUES (5, 'Communication & Media Studies');
INSERT INTO faculty VALUES (6, 'Education');
INSERT INTO faculty VALUES (7, 'Law');
INSERT INTO faculty VALUES (8, 'Medicine');
# --------------------------------------------------------

#
# Tablo için tablo yapýsý `ftp`
#

CREATE TABLE ftp (
  id int(1) unsigned NOT NULL auto_increment,
  Host varchar(220) NOT NULL default '',
  Port tinyint(1) unsigned NOT NULL default '21',
  User varchar(20) default NULL,
  Pass varchar(20) default NULL,
  dsc varchar(64) default NULL,
  PRIMARY KEY  (id)
) TYPE=MyISAM;

#
# Tablo döküm verisi `ftp`
#

INSERT INTO ftp VALUES (4, '193.140.47.235', 21, 'VoD', 'ftp', '');
# --------------------------------------------------------

#
# Tablo için tablo yapýsý `project`
#

CREATE TABLE project (
  id int(1) unsigned NOT NULL auto_increment,
  name varchar(100) default NULL,
  chid varchar(10) default NULL,
  subject varchar(200) default NULL,
  projectdesc text,
  VideoPath varchar(255) default NULL,
  VideoName varchar(255) default NULL,
  ftpid int(1) unsigned NOT NULL default '0',
  PRIMARY KEY  (id)
) TYPE=MyISAM;

#
# Tablo döküm verisi `project`
#

INSERT INTO project VALUES (54, 'Fool misguided fool', '14', 'Music', '', '{5A77D9F2-9899-46ab-BC77-DF1D010C3E94}', 'starsailor.mpg', 0);
INSERT INTO project VALUES (53, 'Warriors of the Net', '13', 'Network animation', '', '{4E4D498B-1EDB-43d8-B52C-E66BF859B965}', 'warriors-1150.mpg', 0);
INSERT INTO project VALUES (50, 'IMAX', '7', '', '', '{C3BA4561-5E6C-4254-93ED-51056F812B60}', 'IMAX - Mysteries Of Egypt.avi', 0);
INSERT INTO project VALUES (51, 'IMAX 1', '7', 'egypt', '', '{05183B9C-99E3-416b-9E1A-DA691581C6AD}', 'IMAX - Mysteries Of Egypt.avi', 0);
INSERT INTO project VALUES (47, 'Guns in roses', '5', 'November rain', '', '{B431D550-EEE3-4a1a-ABA9-6E05B8C4C16E}', 'Guns n Roses - November Rain (3).mpg', 0);
INSERT INTO project VALUES (34, 'Travis', '5', 'tttt', '', '{B95AC647-A862-4f11-9BDC-DD220BAF1E41}', 'Travis - Sing - XF.mpg', 0);
INSERT INTO project VALUES (46, 'Shania 2', '8', 'Ka Ching', '', '{E12FCE31-6280-4e9f-ACF4-F7425AB14D80}', 'shania_twain-ka_ching.mpg', 0);
INSERT INTO project VALUES (45, 'Queen', '12', 'I am going slightly mad', '', '{2705B386-D2D8-4d47-A053-EB88DC79FA8B}', 'Queen - Im Going Slightly Mad.mpg', 0);
INSERT INTO project VALUES (37, 'Bina animasyonu 1', '7', 'Textured + Wireframe', '', '{DDC78E1E-C82B-481e-B82A-2581F7132552}', 'Mix2.avi', 0);
INSERT INTO project VALUES (38, 'Bina animasyonu 2', '7', 'Textured + Wireframe together', '', '{D1EBB055-DDFC-4893-9773-38DD8B1E8A42}', 'Mix3.avi', 0);
INSERT INTO project VALUES (39, 'Shania Twain', '8', 'Ka-Ching', '', '{7DB7586A-2530-4c37-A103-EC1B69A897C3}', 'Shania Twain-Ka Ching! (European Video Release).mpg', 0);
INSERT INTO project VALUES (44, 'Madonna', '11', 'You will see', '', '{465BF10D-8CEF-42a9-B4E7-398DFF521A2C}', 'Madonna - You will  See.mpg', 0);
INSERT INTO project VALUES (41, 'rosey', '5', 'love', 'audio only', '{6D4B3F1A-4FCB-4407-BC54-208A155EDCBF}', 'Rosey - love.mp3', 0);
INSERT INTO project VALUES (43, 'Enya', '10', 'Only Time', '', '{9054A035-E342-4e26-8FB3-FCD9ACC4B4EA}', 'Enya - Only Time.mpg', 0);
INSERT INTO project VALUES (55, 'Nirvana', '15', 'Where did you sleep last night', '', '{B5262481-438D-4348-A1A9-B6E0B029EFC3}', 'Nirvana - sleep last night.mp3', 0);
INSERT INTO project VALUES (56, 'Judas', '16', 'Pain Killer', '', '{2B471F9C-E16B-4fcd-B83E-7C6D731BE0AA}', 'Judas Priest - Painkiller.mp3', 0);
INSERT INTO project VALUES (57, 'Alive', '17', 'Video', '', '{18A01E30-6165-4768-9CCD-FF97AA04C9C3}', 'POD-alive.mpeg', 0);
INSERT INTO project VALUES (58, 'Network Infrastructure', '13', 'test', '', '{A7EB5A28-2B16-4924-BD3D-4AF22A1DC346}', 'Travis - Sing - XF.mpg', 0);
INSERT INTO project VALUES (59, 'door', '6', 'people', '', '{27817A49-61EE-4810-BB4D-4970CFCFA67F}', 'Doors_-_People_Are_Strange.mpg', 0);
INSERT INTO project VALUES (60, 'Guitar lesson', '18', 'Yngwie Malmsteen', '', '{120F1AFD-50F3-460a-BA3A-125FCFBB4288}', 'Guitar Lesson - Yngwie Malmsteen - Hot Licks.mpg', 0);
INSERT INTO project VALUES (61, 'tori', '5', 'amos', '', '{CC65DE0D-F81F-4b24-8955-55DF85D765E4}', 'Tori Amos - Cornflake Girl.mpg', 0);
INSERT INTO project VALUES (62, 'Bring me to life', '19', 'EVANESCENCE', '', '{519F34E6-E178-462d-ABA0-7CE15BC9AC2B}', 'Evanscence - Bring Me To Life.wmv', 0);
INSERT INTO project VALUES (63, 'shoop shoop song', '20', 'test', '', '{0CEEF273-8027-4af2-83CC-D9CCF53F50E8}', 'Cher - Shoop Shoop Song (Its In His Kiss).avi', 0);
INSERT INTO project VALUES (64, 'test 12 12', '6', 'prezentasyon', '', '{3179F146-86E0-4a51-ABC4-992A1F4FBDD5}', 'Dire Straits - Local hero - wild theme.mpg', 0);
INSERT INTO project VALUES (65, 'Toms hardware guide', '21', 'CPU water cooling', '', '{20259E67-EF85-49ce-9F88-3DD269A2974E}', 'THG_CPU_Cooling.avi', 0);
INSERT INTO project VALUES (66, '2 pages', '22', '1 kb', '', '{F622ECD0-EFCB-445f-968C-2A31B56ABA0B}', 'habla me.mp3', 0);
INSERT INTO project VALUES (67, '10 Pages', '22', '', '', '{CC7D3834-8373-45c3-B45A-BE5EE7FCBDE8}', 'habla me.mp3', 0);
INSERT INTO project VALUES (68, '20 Pages', '22', '20 pages', '', '{3010A384-76A5-46aa-88CD-7E6B6A8F23C0}', 'habla me.mp3', 0);
INSERT INTO project VALUES (69, '30 Pages', '22', '30 Pages', '', '{26CE096B-0213-487d-8C08-1D371282492C}', 'habla me.mp3', 0);
# --------------------------------------------------------

#
# Tablo için tablo yapýsý `user`
#

CREATE TABLE user (
  userid int(1) unsigned NOT NULL auto_increment,
  user varchar(50) default NULL,
  pass varchar(50) default NULL,
  privilages tinyint(1) unsigned NOT NULL default '0',
  PRIMARY KEY  (userid)
) TYPE=MyISAM;

#
# Tablo döküm verisi `user`
#

INSERT INTO user VALUES (2, 'beste', '7705d95558ae9944', 0);
INSERT INTO user VALUES (3, 'mozmus', '442b469e091bd61b', 0);
INSERT INTO user VALUES (4, 'cem', '3fb3a05c165090cd', 0);
INSERT INTO user VALUES (5, 'olmec', '3dbcefb8464cf06d', 0);
INSERT INTO user VALUES (6, 'test', '25343c0409f13694', 0);
INSERT INTO user VALUES (7, 'caner', '36a68a1b61a250ff', 0);
INSERT INTO user VALUES (8, 'deniz', '4091d4a62454b57a', 0);

