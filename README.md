# VoD (Video on Demand) - Pedagogically Enhanced Distance Learning System

> **A groundbreaking educational video streaming platform built in 2001-2004, predating YouTube (2005) and Netflix streaming (2007)**

[![IEEE Paper](https://img.shields.io/badge/IEEE-10.1109%2FICALT.2004.1358207-blue)](https://ieeexplore.ieee.org/document/1358207)
[![Year](https://img.shields.io/badge/Year-2001--2004-orange)]()
[![Language](https://img.shields.io/badge/Language-C%2B%2B-red)]()

## Historical Significance

This Master's thesis project represents one of the **earliest educational video streaming systems with pedagogical enhancements**, developed between 2001-2004 at Eastern Mediterranean University - significantly before:
- YouTube (launched February 2005)
- Netflix streaming (started 2007)
- Modern LMS video integration systems
- HTML5 video standards

## Academic Publication

**D.Z. Deniz and C. Karaca**, "Pedagogically enhanced video-on-demand based learning system," *4th IEEE International Conference on Advanced Learning Technologies*, Joensuu, Finland, 2004, pp. 415-420.
**DOI:** [10.1109/ICALT.2004.1358207](https://ieeexplore.ieee.org/document/1358207)

**Full Thesis:** [Tez.pdf](Tez.pdf) (Turkish)

## Project Overview

PeVoD (Pedagogically Enhanced Video on Demand) is a comprehensive **distance learning platform** featuring:

### Core Features
- **Real-time video streaming** with custom protocols (TCP/UDP/RDUP)
- **Teacher control interface** with WYSIWYG HTML content editor
- **Student player application** with VCR-like controls
- **Integrated chat system** for real-time collaboration
- **Discussion forums** and threaded conversations (phpBB-based)
- **Web-based management** interface (PHP/MySQL)
- **MySQL backend** for user and content management
- **Synchronized HTML content** - additional learning materials appear at specific video timestamps
- **NTFS Hidden Streams** - secure video storage using alternate data streams

## Architecture

### System Components

```
VoD System Architecture
├── Client Side
│   ├── VoD Player (Student Interface)
│   │   └── Basic video playback with synchronized content
│   ├── VoD Player Plus (VoDPP)
│   │   └── Enhanced player with integrated chat
│   └── VoD Project Editor (Teacher Interface)
│       ├── WYSIWYG DHTML Editor
│       ├── Timeline Synchronization
│       └── FTP Publishing
│
└── Server Side
    ├── Mediation Server (Web Interface)
    │   ├── PHP Authentication System
    │   ├── MySQL User Management
    │   ├── phpMyAdmin (Database Admin)
    │   ├── phpBB Forums
    │   └── Content Routing
    │
    └── Content Servers (Distributed)
        ├── TCP Streaming Server
        ├── UDP Streaming Server (RDUP)
        ├── FTP Server
        └── Video Library Storage
```

### Technology Stack

- **Frontend**: Windows MFC, DirectX for video rendering
- **Backend**: Custom C++ TCP/UDP/RDUP servers
- **Database**: MySQL 4.x/5.x
- **Web Interface**: PHP, JavaScript, CSS
- **Protocols**: Custom RDUP (Reliable UDP), TCP, UDP, FTP
- **Video Storage**: NTFS Hidden Streams (for privacy)
- **Network**: Distributed server architecture

## Folder Structure

### `/VoD` - VoD Project Editor (Teacher Interface)
The main teacher application for creating and managing video lectures with synchronized content.

**Key Components:**
- **WYSIWYG DHTML Editor** (`Design.cpp`, `Design.h`)
  - Based on MFC's `CHtmlEditView`
  - Full HTML editing with formatting toolbar
  - Commands: Bold, Italic, Underline, Font, Hyperlink, Image, Lists, Alignment
  - Real-time preview capability

- **HTML Viewer** (`Html.cpp`, `Html.h`)
  - Based on `CHtmlView` for previewing content
  - Synchronized with video timeline

- **Source Editor** (`Source.cpp`, `Source.h`)
  - Raw HTML editing via `CEditView`
  - Three-way editing: Design view, Preview, and Source code

- **Project Tree Structure** (`TabProject.h`, `Dialogs.h`)
  - Hierarchical organization: Faculty → Department → Course → Chapter → Project
  - Dialog-based management for each level

- **FTP Publishing** (`Ftpd.cpp`, `FtpDown.cpp`, `NewFtp.cpp`)
  - Deploy projects to content servers
  - Automated synchronization

- **Authentication** (`Login.h`)
  - Teacher login system
  - MySQL-based user management

**Technologies:** MFC, DirectX, MySQL Client, FTP, CHtmlEditView

---

### `/VoDPlayer` - Student Player (Basic Version)
Standalone video player for students without chat functionality.

**Features:**
- DirectX-based video rendering
- VCR controls (play, pause, stop, seek)
- Progress bar with buffering indicator
- Synchronized HTML content display
- Bookmark management
- Drag-and-drop content addition
- Visual timeline markers:
  - Blue: Instructor-added content
  - Green: Student-added bookmarks
- TCP/UDP/RDUP protocol support

**Technologies:** MFC, DirectX, Windows Sockets, CHtmlView

---

### `/VoDPP` - VoD Player Plus (Enhanced Version)
Extended student player with integrated real-time chat capabilities.

**Additional Features:**
- All features from VoDPlayer
- **Real-time chat panel** (`Video.cpp` contains chat references)
- Multi-user collaboration
- Online presence indicators
- Enhanced UI for collaboration

**Technologies:** Same as VoDPlayer + Chat integration

---

### `/VoDTCPServer` - TCP Streaming Server
Dedicated TCP-based video streaming server.

**Responsibilities:**
- TCP socket-based video delivery
- Connection management
- Bandwidth management
- Client authentication
- Session tracking

**Technologies:** MFC, Windows Sockets (TCP)

---

### `/VoDUDPServer` - UDP/RDUP Streaming Server
UDP and RDUP (Reliable UDP) streaming server implementation.

**Responsibilities:**
- UDP-based video streaming
- **RDUP protocol implementation**:
  - 32-bit CRC error checking
  - Packet recovery mechanism
  - Connection-oriented over UDP
  - 9.14 Mbps throughput (25% faster than TCP in tests)
- Low-latency delivery
- Packet loss handling

**Technologies:** MFC, Windows Sockets (UDP), Custom RDUP Protocol

---

### `/vodWeb` - Web-based Management Interface
Complete web frontend for system administration and user interaction.

**Structure:**
```
vodWeb/
├── Admin/              # phpMyAdmin - MySQL database management
├── forum/              # phpBB - Discussion forum system
├── thread/             # Custom discussion threads
├── adm/                # Administrative tools
├── cgi-bin/            # CGI scripts
├── images/             # Web assets
├── libs/               # PHP libraries
├── projects/           # Published video projects
└── vod/                # VoD-specific web files
```

**Key Files:**
- `admin.php` - Administrative dashboard
- `login.php` - User authentication
- `chat.php` - Web-based chat interface
- `list.php` - Content listing
- `menu.php` - Navigation system
- `panel*.php` - Control panels
- `vod.js.php` - JavaScript functionality
- `config.cgi` - Configuration management
- `httpd.conf` - Apache web server configuration
- `vod.sql`, `vod_web.sql` - Database schemas

**Technologies:** PHP, MySQL, JavaScript, phpBB, phpMyAdmin, Apache

---

### Database Files
- `vod.sql` - Main VoD database schema
- `vod_web.sql` - Web interface database schema
- `vodWeb/Admin/scripts/create_tables.sql` - Table creation scripts

---

### Documentation
- `Tez.pdf` - Full Master's thesis (Turkish, 2004)
- `tezPres.pdf` - Thesis presentation slides
- IEEE Paper - Published at ICALT 2004 conference

---

## Revolutionary Features

### 1. **RDUP Protocol** (Reliable UDP)
Custom-designed streaming protocol specifically for video delivery:
- **Connection-oriented** unlike standard UDP
- **32-bit CRC error checking**
- **Packet recovery** mechanism
- **9.14 Mbps throughput** (25% faster than TCP in testing)
- Designed for low bit-error-rate networks

**Performance Results** (10 Mbps ATM Network):
| Protocol | Speed (Mbps) | Overhead |
|----------|-------------|----------|
| FTP      | 7.33        | Highest  |
| TCP      | 7.69        | High     |
| UDP      | 8.63        | Low      |
| **RDUP** | **9.14**    | **Optimized** |

### 2. **AVI Header Innovation**
A **breakthrough moment** (literally conceived in the bathroom!):

**The Problem:** AVI files couldn't start playing until fully downloaded because header information was at the end.

**The Solution:**
```
1. Allocate full file size on client
2. Download last ~2KB of video first (contains header)
3. Then stream from beginning
4. Result: Videos play while still downloading!
```

This was critical for enabling true streaming of AVI files in 2001 when:
- No HTTP progressive download
- No modern streaming codecs
- Limited bandwidth (dial-up era)

### 3. **NTFS Hidden Streams Security**
Videos were stored in **NTFS Alternate Data Streams** for privacy:
- Hidden from normal file browsing
- Protected from unauthorized copying
- Innovative use of NTFS features for DRM-like protection
- Cutting-edge technique for 2001!

### 4. **Smooth Buffering Algorithm**
Mathematical model for determining optimal playback start time:

```
When to start playback:
- If download speed > playback speed: Start immediately
- If download speed < playback speed: Buffer until safe threshold
- Dynamic adjustment based on network conditions
```

### 5. **Pedagogical Enhancements**

Unlike simple video players, PeVoD provided:
- **Synchronized learning materials** - HTML pages appear at specific timestamps
- **Student annotations** - learners can add their own notes/links to videos
- **Drag-and-drop content** - pause video and drop documents to save at that timestamp
- **Discussion integration** - per-video threaded discussions
- **Real-time chat** - students online simultaneously can discuss content

### 6. **WYSIWYG DHTML Editor**
Built into the VoD Project Editor for teachers to create synchronized content:

**Based on MFC's CHtmlEditView** (`VoD/Design.cpp`, `VoD/Design.h`):
- Full **What-You-See-Is-What-You-Get** editing
- Rich formatting toolbar with buttons for:
  - Text formatting: Bold, Italic, Underline
  - Font selection and sizing
  - Text alignment: Left, Center, Right
  - Lists: Ordered and Unordered
  - Indentation controls
  - Hyperlink insertion
  - Image embedding
- **Three editing modes**:
  1. **Design View** - WYSIWYG visual editing
  2. **Preview View** - See exactly how students will see it
  3. **Source View** - Raw HTML code editing
- **Timeline integration** - Content automatically synchronized to video timestamps
- **Auto-save functionality** - Changes preserved automatically
- **Template system** - Reusable content templates

**Technical Implementation:**
```cpp
// Based on MFC's CHtmlEditView
class CDesign : public CHtmlEditView
{
    CToolBar m_wndFormatBar;  // Formatting toolbar

    // DHTML editing command map
    BEGIN_DHTMLEDITING_CMDMAP(CDesign)
        DHTMLEDITING_CMD_ENTRY(ID_BOLD, IDM_BOLD)
        DHTMLEDITING_CMD_ENTRY(ID_ITALIC, IDM_ITALIC)
        DHTMLEDITING_CMD_ENTRY(ID_UNDERLINE, IDM_UNDERLINE)
        DHTMLEDITING_CMD_ENTRY(ID_HYPERLINK, IDM_HYPERLINK)
        DHTMLEDITING_CMD_ENTRY(ID_IMAGE, IDM_IMAGE)
        // ... more commands
    END_DHTMLEDITING_CMDMAP()
};
```

This was **advanced for 2001-2004**:
- Most educational systems used plain text or basic HTML forms
- WYSIWYG web editors were rare and expensive
- Microsoft FrontPage was the main alternative (bloated and complex)
- VoD's editor was streamlined specifically for educational content creation

---

## System Interfaces

### VoD Player (Student Interface)
**Features:**
- Main video screen with DirectX rendering
- HTML browser pane for synchronized content
- Progress bar showing buffered vs played content
- Visual indicators for:
  - Blue markers: Instructor-added content points
  - Green markers: Student-added bookmarks
- VCR controls (play/pause/stop/seek)
- Drag-and-drop content addition
- Chat panel (in VoDPP version)
- Discussion forum access

### VoD Project Editor (Teacher Interface)
**Features:**
- Video preview screen
- Project tree (Faculty → Department → Course → Lecture)
- Content timeline ruler with synchronization markers
- **WYSIWYG DHTML editor** with formatting toolbar
- Three-way editing (Design/Preview/Source)
- Synchronization tools
- Build and publish functionality
- FTP deployment to content servers
- User management interface

### Web Management Interface
**Features:**
- User authentication and registration
- Course catalog browsing
- Video library management
- phpBB discussion forums
- Real-time chat rooms
- Administrative control panel
- Database management (phpMyAdmin)
- Usage statistics and reporting

---

## Installation & Setup

> **Note**: This is archival code from 2004. Some dependencies are not included.

### Prerequisites
- Windows OS (XP/2003 era recommended for compatibility)
- MySQL Server 4.x/5.x
- DirectX SDK (for video playback)
- Visual Studio 2003/2005
- Apache/IIS web server (for vodWeb)
- PHP 4.x/5.x
- ATM network (optional, for RDUP testing)

### Database Setup
```bash
# Import database schemas
mysql -u root -p < vod.sql
mysql -u root -p < vod_web.sql
```

### Build Instructions
1. Open `VoD.sln` in Visual Studio
2. Configure MySQL include paths
3. Build server components first:
   - VoDTCPServer
   - VoDUDPServer
4. Build client applications:
   - VoD (Project Editor)
   - VoDPlayer
   - VoDPP (Player Plus)
5. Deploy web interface:
   - Copy `vodWeb/` to web server document root
   - Configure `vodWeb/config.php` with database credentials
   - Set up phpBB forum
6. Deploy to appropriate servers

### Network Configuration
- **Mediation Server**: Central authentication (MySQL + Web server)
- **Content Servers**: Distributed (TCP/UDP/RDUP/FTP services)
- **Clients**: VoD Player or Project Editor

---

## Educational Value

This repository serves as an invaluable reference for:

### Software Engineering History
- **Pre-cloud distributed systems** design patterns
- Early **streaming video** architecture
- Custom **protocol design** (RDUP)
- **Client-server** patterns from the early 2000s

### Technology Topics
- **MFC Windows programming** with DirectX
- **Network programming** - TCP/UDP sockets
- **Custom application protocols**
- **Database-driven architecture**
- **Multimedia streaming** concepts
- **NTFS internals** - alternate data streams
- **WYSIWYG HTML editing** with CHtmlEditView
- **ActiveX and COM** integration

### Educational Technology
- **Asynchronous vs Synchronous** learning systems
- **Pedagogical design** for video-based education
- **User interaction patterns** in e-learning
- **Content synchronization** techniques
- **Collaborative learning** tools

---

## Documentation

- **[Full Thesis](Tez.pdf)** (Turkish, 2004) - Complete technical documentation
- **[Thesis Presentation](tezPres.pdf)** - Overview slides
- **[IEEE Paper](https://ieeexplore.ieee.org/document/1358207)** - Peer-reviewed publication

---

## Research Contributions

1. **RDUP Protocol Design** - Reliable UDP optimized for video streaming
2. **AVI Streaming Solution** - Header-at-end problem solved
3. **Smooth Buffering Algorithm** - Mathematical model for playback timing
4. **Pedagogical Framework** - Synchronized multimedia learning materials
5. **Distributed Architecture** - Multi-server content delivery (pre-CDN)
6. **WYSIWYG Content Creation** - Teacher-friendly interface for course material development

---

## Innovation Context (2001-2004)

When this system was built:
- **No YouTube** - video sharing didn't exist
- **No Netflix streaming** - DVDs by mail only
- **Dial-up dominant** - 56K modems common
- **No Flash video** - limited web video options
- **No smartphones** - mobile video wasn't a thing
- **No HTML5** - video plugins required
- **No CDNs** - no Cloudflare, Akamai was expensive
- **No WYSIWYG web editors** - FrontPage was the main alternative

Yet this system implemented:
- Multi-protocol streaming
- Distributed content delivery
- Interactive features
- Custom reliable UDP
- Synchronized multimedia
- Educational collaboration tools
- **WYSIWYG HTML content editor**

---

## Author

**Cem (Cihan) KARACA**
Master's Thesis Project (2001-2004)
Eastern Mediterranean University
Supervisor: Assoc. Prof. Dr. Derviş Z. DENİZ

---

## License

This project is provided for **educational and historical reference purposes**.

---

## Recognition

This system demonstrated concepts that would become standard in modern e-learning:
- Video-centric learning (now: Coursera, edX, Udemy)
- Synchronized materials (now: Interactive video platforms)
- Real-time collaboration (now: Zoom, Teams education features)
- Distributed streaming (now: CDNs, adaptive bitrate)
- WYSIWYG content creation (now: Canvas, Moodle editors)

---

## Keywords

`video on demand`, `distance education`, `e-learning`, `streaming protocols`, `RDUP`, `pedagogical enhancement`, `MFC programming`, `DirectX`, `distributed systems`, `early 2000s technology`, `pre-YouTube`, `educational technology`, `Windows programming`, `custom protocols`, `AVI streaming`, `NTFS hidden streams`, `WYSIWYG editor`, `DHTML`, `CHtmlEditView`, `phpBB`, `MySQL`

---

**If you appreciate this piece of computing history, please star the repository!**

*"Sometimes the best ideas come in the most unexpected places - like realizing AVI headers are at the end of the file while in the bathroom!"*
