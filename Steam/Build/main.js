const { app, BrowserWindow } = require('electron');
const path = require('path');

function createWindow(){
  const win = new BrowserWindow({
    width: 1280,
    height: 720,
    minWidth: 1024,
    minHeight: 600,
    title: "Sultan Palace - قصر السلطان - Topkapi 1481",
    icon: path.join(__dirname, 'SultanPalace/assets/courtyard_aaa.jpg'),
    webPreferences: {
      nodeIntegration: false,
      contextIsolation: true,
      enableRemoteModule: false
    },
    backgroundColor: '#05080f',
    show: false
  });
  win.maximize();
  win.show();
  win.loadFile(path.join(__dirname, 'SultanPalace/index.html'));
  // Uncomment for devtools
  // win.webContents.openDevTools();
  win.setMenuBarVisibility(false);
}

app.whenReady().then(createWindow);
app.on('window-all-closed', ()=>{ if(process.platform!=='darwin') app.quit(); });
app.on('activate', ()=>{ if(BrowserWindow.getAllWindows().length===0) createWindow(); });
