ans = null;
internal = {
    enable_cmd_stk: true,
    enable_script_stk: false,
    command_stack: [],
    command_script_stack: []
}


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

btn_save_png = new gui.Button({
    icon: ':/icons/filetype/png',
    text: 'Lossless',
    onclick: ()=> {
        if(!CheckAns()) return;
        ans.write('output' + '.png')
    }
});

btn_save_jpg = new gui.Button({
    icon: ':/icons/filetype/jpg',
    text: 'Lossy',
    onclick: ()=> {
        if(!CheckAns()) return;
        ans.write('output' + '.jpg')
    }
});

btn_save_bmp = new gui.Button({
    icon: ':/icons/filetype/bmp',
    text: 'Bitmap',
    onclick: ()=> {
        if(!CheckAns()) return;
        ans.write('output' + '.bmp')
    }
});

btn_save_tif = new gui.Button({
    icon: ':/icons/filetype/tif',
    text: 'TIF',
    onclick: ()=> {
        if(!CheckAns()) return;
        ans.write('output' + '.tif')
    }
});

btn_undo = new gui.Button({
    icon: ':/icons/edit/undo',
    text: 'Undo',
    onclick: ()=> {
        if(!CheckAns()) return;
    }
});

btn_redo = new gui.Button({
    icon: ':/icons/edit/redo',
    text: 'Redo',
    onclick: ()=> {
        if(!CheckAns()) return;
    }
});

btn_read = new gui.Button({
    icon: ':/icons/live_folder_2.svg',
    text: 'Open...',
    onclick: ()=> {
        ans = new Matrix;
        gui.fileDialog({
            baseDir: '.',
            nameFilter: 'Images(*.png *.jpg *.jpeg *.bmp *.tif)',
            title: 'Open Image...',
            callback: (arr) => {
                if(arr.length > 0) {
                    ans.read(arr[0])
                    ans['filename'] = arr[0]
                }
            }
        })
        gui.display(ans)
    }
})


vw_global = new gui.ObjectViewer();
btn_refresh_obj = new gui.Button({
    icon: ':/icons/refresh_doc',
    text: 'Refresh',
    onclick: ()=> {
        vw_global = new gui.ObjectViewer();
    }
})


function CheckAns() {
    if(ans == null) {
        console.log('Image is not loaded');
        return false;
    }
    return true;
}

function RotateCallback(degree) {
    if(!CheckAns()) return;

    if(ans['rotate'] === undefined || ans['rotate'] === null) {
        ans['rotate'] = 0;
    }

    var delta = degree - ans['rotate'];
    ans.rotate(delta);
}

sld_rotate = new gui.Slider({
    max: 180,
    min: -180,
    text: 'Rotate',
    onslide: (degree)=>{
        //shell.print('degree: ', degree, ' delta:', delta)
        if(!CheckAns()) return;
    
        if(typeof(ans.rotate) === 'undefined') {
            ans['rotate'] = 0;
        }
    
        var delta = degree - ans['rotate'];
        shell.print('degree: ', degree, ' delta: ', delta)
        ans.rotate(delta);
        
    }
})
/*
ckbx_setting_rec_cmd = new gui.Checkbox({
    text: 'Record Command',
    onclick: (boolean) => {
        internal.enable_cmd_stk = boolean
    }
})

ckbx_setting_rec_script = new gui.Checkbox({
    text: 'Record Script',
    onclick: (boolean) => {
        internal.enable_script_stk = boolean
    }
})
gui.ribbon.addWild(ckbx_setting, 'Settings', 'Commands')
gui.ribbon.addWild(ckbx_setting, 'Settings', 'Commands')
*/

// gui.ribbon.add(target button, Tab name, group name)

gui.ribbon.add(btn_read, 'Project', 'Files');
gui.ribbon.add(btn_save_jpg, 'Project', 'Save as');
gui.ribbon.add(btn_save_png, 'Project', 'Save as');
gui.ribbon.add(btn_save_bmp, 'Project', 'Save as');
gui.ribbon.add(btn_save_tif, 'Project', 'Save as');
gui.ribbon.add(btn_refresh_obj, 'Project', 'Object Viewer');

gui.ribbon.add(btn_undo, 'Edit', 'Navigate');
gui.ribbon.add(btn_redo, 'Edit', 'Navigate');

gui.ribbon.add(btn_gray, 'Edit', 'Mono');
gui.ribbon.add(btn_bin, 'Edit', 'Mono');

gui.ribbon.addSlider(sld_rotate, 'Edit', 'Rotate');