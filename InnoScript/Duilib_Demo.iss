; 脚本用 Inno Setup 脚本向导 生成。
; 查阅文档获取创建 INNO SETUP 脚本文件的详细资料！

#define MyAppName "Dr. Wise Pacs Download"
#define MyAppVersion "V1.0.0.0"
#define MyAppPublisher "Deepwise, Inc."
#define MyAppURL "http://www.deepwise.com/"
#define MyAppExeName "Dr. Wise Pacs Download.exe"
#define MyDateTimeString GetDateTimeString('yyyy-mm-dd-hh-mm-ss', '', '')
#define AppVersion GetFileVersion("..\Bin\Dr. Wise Pacs Download.exe")
#define AppDesc GetStringFileInfo("..\Bin\Dr. Wise Pacs Download.exe", PRODUCT_VERSION)

[Setup]
; 注意: AppId 的值是唯一识别这个程序的标志。
; 不要在其他程序中使用相同的 AppId 值。
; (在编译器中点击菜单“工具 -> 产生 GUID”可以产生一个新的 GUID)
AppId={{BD78FECE-86F4-4050-9C08-45278F2E1433}
AppName={#MyAppName}
AppVersion={#MyAppVersion}
;AppVerName={#MyAppName} {#MyAppVersion}
AppPublisher={#MyAppPublisher}
AppPublisherURL={#MyAppURL}
AppSupportURL={#MyAppURL}
AppUpdatesURL={#MyAppURL}
DefaultDirName={pf}\{#MyAppName}
DefaultGroupName={#MyAppName}
OutputBaseFilename={#MyAppName}_{#MyDateTimeString}_{#AppDesc}
Compression=lzmaSetupIconFile=ginkgo256x256.ico
OutputDir=..\Output
SolidCompression=yes

[Languages]
Name: "chinesesimp"; MessagesFile: "compiler:Languages\ChineseSimp.isl"

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked

[Files]
Source: "..\Bin\Dr. Wise Pacs Download.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\Bin\locales\*"; DestDir: "{app}\locales\"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "..\Bin\Skin\common_icon\*"; DestDir: "{app}\skin\common_icon"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "..\Bin\Skin\frame_wnd_demo\*"; DestDir: "{app}\skin\frame_wnd_demo"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "..\Bin\Skin\image\*"; DestDir: "{app}\skin\image"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "..\Bin\Skin\main\*"; DestDir: "{app}\skin\main"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "..\Bin\Skin\sysbtn\*"; DestDir: "{app}\skin\sysbtn"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "..\Bin\Skin\window_title\*"; DestDir: "{app}\skin\window_title"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "..\Bin\Skin\rightmenu\*"; DestDir: "{app}\skin\rightmenu"; Flags: ignoreversion recursesubdirs createallsubdirs

Source: "..\Bin\natives_blob.bin"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\Bin\snapshot_blob.bin"; DestDir: "{app}"; Flags: ignoreversion
;Source: "..\Bin\v8_context_snapshot.bin"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\Bin\icudtl.dat"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\Bin\deepwise_pacs_download_config.ini"; DestDir: "{app}"; Flags: ignoreversion
;Source: "..\Bin\cef_sandbox.lib"; DestDir: "{app}"; Flags: ignoreversion

Source: "..\Bin\conformance.xml"; DestDir: "{app}"; Flags: ignoreversion
;Source: "..\Bin\cef.pak"; DestDir: "{app}"; Flags: ignoreversion
;Source: "..\Bin\cef_100_percent.pak"; DestDir: "{app}"; Flags: ignoreversion
;Source: "..\Bin\cef_200_percent.pak"; DestDir: "{app}"; Flags: ignoreversion
;Source: "..\Bin\cef_extensions.pak"; DestDir: "{app}"; Flags: ignoreversion
;Source: "..\Bin\chrome_elf.dll"; DestDir: "{app}"; Flags: ignoreversion
;Source: "..\Bin\d3dcompiler_43.dll"; DestDir: "{app}"; Flags: ignoreversion
;Source: "..\Bin\d3dcompiler_47.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\Bin\DuiLib.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\Bin\DuiLib_u.dll"; DestDir: "{app}"; Flags: ignoreversion
;Source: "..\Bin\libcef.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\Bin\libEGL.dll"; DestDir: "{app}"; Flags: ignoreversion
;Source: "..\Bin\libGLESv2.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\Bin\widevinecdmadapter.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\Bin\ISTask.dll"; DestDir: "{app}"; Flags: ignoreversion

[Icons]
Name: "{group}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"
Name: "{commondesktop}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"; Tasks: desktopicon

[Run]
Filename: "{app}\{#MyAppExeName}"; Description: "{cm:LaunchProgram,{#StringChange(MyAppName, '&', '&&')}}"; Flags: nowait postinstall skipifsilent

[code]
//安装前判断是否有进程正在运行，istask.dll文件放到innosetup安装目录
function RunTask(FileName: string; bFullpath: Boolean): Boolean;
external 'RunTask@files:ISTask.dll stdcall delayload';
function KillTask(ExeFileName: string): Integer;
external 'KillTask@files:ISTask.dll stdcall delayload';

function InitializeSetup(): Boolean;
begin
  Result:= true;
  if RunTask('{#MyAppExeName}', false) then
      begin
        KillTask('{#MyAppExeName}');
        Result:= true;  
      end
  else
  Result:= true;
end;

// 卸载前判断进程是否在运行，istask.dll文件与打包的exe文件一起
function RunTaskU(FileName: string; bFullpath: Boolean): Boolean;
external 'RunTask@{app}/ISTask.dll stdcall delayload uninstallonly';
function KillTaskU(ExeFileName: string): Integer;
external 'KillTask@{app}/ISTask.dll stdcall delayload uninstallonly';

function InitializeUninstall(): Boolean;
  begin
    Result:= true;
    if RunTaskU('{#MyAppExeName}', false) then
      begin
        if MsgBox('软件正在运行中，是否关闭软件并卸载！', mbConfirmation, MB_YESNO) = IDYES then
          begin
            KillTaskU('{#MyAppExeName}');
            Result:= true;
          end
        else
          begin
            Result :=false; //安装程序退出  
          end
      end
    else
    Result:= true;
    UnloadDll(ExpandConstant('{app}/ISTask.dll'));
end;