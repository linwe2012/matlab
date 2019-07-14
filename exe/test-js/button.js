ans = null;

btn_bin = new gui.Button(
    {
        icon: ':/icons/edit/bin',
        text: 'Binerize',
        onclick: ()=> {
            if(!CheckAns()) return;
            ans.tobin();
            gui.display(ans)
        }
    }
);


btn_gray = new gui.Button(
    {
        icon: ':/icons/edit/gray',
        text: 'Mono Color',
        onclick: ()=> {
            if(!CheckAns()) return;
            ans.togray();
            gui.display(ans)
        }
    }
);

btn_save_png = new gui.Button(
    {
        icon: ':/icons/filetype/png',
        text: 'Lossless',
        onclick: ()=> {
            if(!CheckAns()) return;
            ans.write('output' + '.png')
        }
    }
);

btn_save_jpg = new gui.Button(
    {
        icon: ':/icons/filetype/jpg',
        text: 'Lossy',
        onclick: ()=> {
            if(!CheckAns()) return;
            ans.write('output' + '.jpg')
        }
    }
);

btn_save_bmp = new gui.Button(
    {
        icon: ':/icons/filetype/bmp',
        text: 'Bitmap',
        onclick: ()=> {
            if(!CheckAns()) return;
            ans.write('output' + '.bmp')
        }
    }
);

btn_save_tif = new gui.Button(
    {
        icon: ':/icons/filetype/tif',
        text: 'TIF',
        onclick: ()=> {
            if(!CheckAns()) return;
            ans.write('output' + '.tif')
        }
    }
);

btn_undo = new gui.Button(
    {
        icon: ':/icons/edit/undo',
        text: 'Undo',
        onclick: ()=> {
            if(!CheckAns()) return;
        }
    }
);

btn_redo = new gui.Button(
    {
        icon: ':/icons/edit/redo',
        text: 'Redo',
        onclick: ()=> {
            if(!CheckAns()) return;
        }
    }
);

btn_read = new gui.Button(
    {
        icon: ':/icons/live_folder_2.svg',
        text: 'Open...',
        onclick: ()=> {
            ans = new Matrix;
            gui.fileDialog(
                {
                    baseDir: '.',
                    nameFilter: 'Images(*.png *.jpg *.jpeg *.bmp)',
                    title: 'Open Image...',
                    callback: (arr) => {
                        if(arr.length) {
                            ans.read(arr[0])
                        }
                    }
                }
            )
            // ans.read('test-js/Lena.bmp')
            gui.display(ans)
        }
    }
)


vw_global = new gui.ObjectViewer();


function CheckAns() {
    if(ans == null) {
        console.log('Image is not loaded');
        return false;
    }
    return true;
}

gui.ribbon.add(btn_read, 'Project', 'Files');
gui.ribbon.add(btn_save_jpg, 'Project', 'Save as');
gui.ribbon.add(btn_save_png, 'Project', 'Save as');
gui.ribbon.add(btn_save_bmp, 'Project', 'Save as');
gui.ribbon.add(btn_save_tif, 'Project', 'Save as');

gui.ribbon.add(btn_undo, 'Edit', 'Navigate');
gui.ribbon.add(btn_redo, 'Edit', 'Navigate');

gui.ribbon.add(btn_gray, 'Edit', 'Mono');
gui.ribbon.add(btn_bin, 'Edit', 'Mono');
