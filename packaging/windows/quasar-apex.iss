; Inno Setup script (skeleton)
[Setup]
AppName=Quasar Apex
AppVersion=10.0.0
DefaultDirName={pf}\Quasar Apex
DefaultGroupName=Quasar Apex
Compression=lzma
SolidCompression=yes

[Files]
Source: "build\apps\quasar-grep.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "build\apps\quasar-ls.exe";   DestDir: "{app}"; Flags: ignoreversion

[Icons]
Name: "{group}\Quasar Grep"; Filename: "{app}\quasar-grep.exe"
Name: "{group}\Quasar LS";   Filename: "{app}\quasar-ls.exe"
