ans = null;
internal = {
    enable_cmd_stk: true,
    enable_script_stk: false,
    command_stack: [],
    command_stack_cursor: 0,
    command_script_stack: [],
    enable_refresh_img: true,

    add_cmd(obj, src) {
        if(this.enable_cmd_stk) {
            if(this.command_stack_cursor < this.command_stack.length) {
                this.command_stack[this.command_stack_cursor] = obj
                this.command_script_stack[this.command_stack_cursor] = {
                    src: src,
                    checked: this.enable_script_stk
                }
            }
            else {
                this.command_stack.push(obj);
                this.command_script_stack.push({
                    src: src,
                    checked: this.enable_script_stk
                });
                shell.print(this.command_script_stack[this.command_stack_cursor].src)
            }
            
            ++this.command_stack_cursor;
        }
    }
}

function RefreshImg(img) {
    if(internal.enable_refresh_img) {
        gui.display(img)
    }
}

function ToBin() {
    if(!CheckAns()) return;
    ans.tobin();
    RefreshImg(ans)
}

btn_bin = new gui.Button(
    {
        icon: ':/icons/edit/bin',
        text: 'Binarize',
        onclick: ()=> {
            if(!CheckAns()) return;
            var s = {
                undo(){
                    ans = this.data;
                    RefreshImg(ans);
                },
                data: ans.clone(),
                redo: ToBin
            }
        
            internal.add_cmd(s, 'ToBin()')
            s.redo()
        }
    }
);
function ToGray() {
    if(!CheckAns()) return;
    ans.togray();
    RefreshImg(ans)
}


btn_gray = new gui.Button(
    {
        icon: ':/icons/edit/gray',
        text: 'Grayscale',
        onclick: ()=> {
            if(!CheckAns()) return;
            internal.add_cmd({
                data: ans.clone(),
                undo(){ ans = this.data; RefreshImg(ans); shell.print('[undo]: to gray')},
                redo: ToGray
            }, 'ToGray()')
            ToGray()
        }
    }
);

function Equalize() {
    if(!CheckAns()) return;
    if(internal.enable_cmd_stk) {
        internal.add_cmd({
            data: ans.clone(),
            undo(){ ans = this.data; RefreshImg(ans); shell.print('[undo]: equalize')},
            redo: Equalize
        }, 'Equalize()')
    }
    ans.equalizeHist();
    RefreshImg(ans);
}

btn_equalize = new gui.Button(
    {
        icon: ':/icons/edit/histogram',
        text: 'Equalize',
        onclick: Equalize
    }
);

function DetectFace() {
    if(!CheckAns()) return;
    if(internal.enable_cmd_stk) {
        internal.add_cmd({
            data: ans.clone(),
            undo(){ ans = this.data; RefreshImg(ans); shell.print('[undo]: equalize')},
            redo: DetectFace
        }, 'DetectFace()')
    }
    ans.face();
    RefreshImg(ans);
}

btn_face = new gui.Button({
    icon: ':/icons/edit/face',
    text: 'Detect Face',
    onclick: DetectFace
});


function WriteImage(type) {

    if(!CheckAns()) return;

    if(internal.enable_cmd_stk) {
        internal.add_cmd({
            undo() {},
            data:  {
                type: 'png'
            },
            redo(){WriteImage(this.data.type)}
        }, 'WriteImage("',type, '")')
    }
    gui.saveAsDialog({
        baseDir: '.',
        nameFilter: 'Images(*.' + type+')',
        title: 'Save As',
        name: shell.replaceExt(ans['filename'] || 'UserImage.png', type),
        callback (arr) {
            if(arr.length > 0) {
                ans.read(arr[0])
                ans['filename'] = arr[0]
                ans.write(arr[0])
            }
        }
    })

    // ans.write('output' + '.' + type)
}

btn_save_png = new gui.Button({
    icon: ':/icons/filetype/png',
    text: 'Lossless',
    onclick: ()=> {
        WriteImage('png')
    }
});

btn_save_jpg = new gui.Button({
    icon: ':/icons/filetype/jpg',
    text: 'Lossy',
    onclick: ()=> {
        WriteImage('jpg')
    }
});

btn_save_bmp = new gui.Button({
    icon: ':/icons/filetype/bmp',
    text: 'Bitmap',
    onclick: ()=> {
        WriteImage('bmp')
    }
});

btn_save_tif = new gui.Button({
    icon: ':/icons/filetype/tif',
    text: 'TIF',
    onclick: ()=> {
        WriteImage('tif')
    }
});

btn_undo = new gui.Button({
    icon: ':/icons/edit/undo',
    text: 'Undo',
    onclick() {
        var i = internal.command_stack_cursor
        if(i > 0) {
            let last = internal.enable_cmd_stk
            internal.enable_cmd_stk = false
            shell.print(internal.enable_cmd_stk)
            shell.print('i=', i)
            --internal.command_stack_cursor
            internal.command_stack[i-1].undo();
            internal.enable_cmd_stk = last;
            shell.print(internal.enable_cmd_stk)
        }
    }
});

btn_redo = new gui.Button({
    icon: ':/icons/edit/redo',
    text: 'Redo',
    onclick() {
        var  i = internal.command_stack_cursor;
        if(i < internal.command_stack.length) {
            let last = internal.enable_cmd_stk
            internal.enable_cmd_stk = false
            internal.command_stack[i].redo();
            ++internal.command_stack_cursor;
            internal.enable_cmd_stk = last;
        }
    }
});

btn_read = new gui.Button({
    icon: ':/icons/live_folder_2.svg',
    text: 'Open...',
    onclick() {
        ans = new Matrix;
        gui.fileDialog({
            baseDir: '.',
            nameFilter: 'Images(*.png *.jpg *.jpeg *.bmp *.tif)',
            title: 'Open Image...',
            callback (arr) {
                if(arr.length > 0) {
                    ans.read(arr[0])
                    ans['filename'] = arr[0]
                }
            }
        })
        gui.display(ans)
    }
})

btn_outputScript = new gui.Button ({
    icon: ':/icons/filetype/js',
    text: 'Save Script',
    onclick() {
        gui.saveAsDialog({
            baseDir: '.',
            nameFilter: '*.js',
            title: 'Save Script As...',
            name: 'script.js',
            callback (arr) {
                if(arr.length > 0) {
                    var _text = '';
                    for(var src of internal.command_script_stack) {
                        shell.print(src.src, ':', src.checked)
                        if(src.checked) {
                            _text = _text + src.src + ';\n';
                        }
                    }
                    shell.writetxt(arr[0], _text);
                }
            }
        })
    }
})

btn_batchjob = new gui.Button ({
    icon: ':/icons/edit/batch',
    text: 'Batch Processing',
    onclick() {
        gui.fileDialog({
            baseDir: '.',
            nameFilter: '*.js',
            title: 'Select a script...',
            callback (arr) {
                if(!(arr.length > 0)) {
                   return
                }
                var src = shell.readtxt(arr[0]);
                internal.enable_cmd_stk = false;

                gui.fileDialog({
                    baseDir: '.',
                    nameFilter: 'Images(*.png *.jpg *.jpeg *.bmp *.tif)',
                    title: 'Select images working on...',
                    multi: true,
                    callback (farr) {
                        if(ans == null) {
                            ans = new Matrix
                        }
                        shell.print(farr)
                        for(var file of farr) {
                            if(!file) {
                                continue
                            }

                            ans.read(file);
                            eval(src)
                            ans.write('./batch/' + shell.getFilename(file))
                        }
                    }
                })
            }
        })
    }
})


btn_open_devtools = new gui.Button({
    icon: ':/icons/edit/console',
    text: 'Open DevTool',
    onclick() {
        gui.Inspector.show()
    }
})

vw_global = new gui.ObjectViewer();
btn_refresh_obj = new gui.Button({
    icon: ':/icons/refresh_doc',
    text: 'Refresh',
    onclick: ()=> {
        vw_global.refreshAll();
    }
})





function CheckAns() {
    if(ans == null) {
        console.log('Image is not loaded');
        return false;
    }
    return true;
}

function RotateCallback(degree, make_record) {
    if(!CheckAns()) return;
    if(typeof(ans.data_rotate) === 'undefined' || ans.data_rotate === null) {
        ans['data_rotate'] = 0;
    }

    if(internal.enable_cmd_stk && make_record) {
        internal.add_cmd({
            redo(){RotateCallback(this.data.redo);},
            undo(){RotateCallback(this.data.undo)},
            data: {
                redo: degree,
                undo: ans.data_rotate
            }
        }, 'RotateCallback(' + degree +  ', true)')
    }
    
    var delta = degree - ans.data_rotate;
    ans.data_rotate = degree;
    ans.rotate(delta);
    RefreshImg(ans)
}

sld_rotate = new gui.Slider({
    max: 180,
    min: -180,
    text: 'Rotate',
    onslide(degree){
        RotateCallback(degree, false)
    },
    onpress() {
        if(!CheckAns()) return;
        if(typeof(ans.data_rotate) === 'undefined' || ans.data_rotate === null) {
            ans['data_rotate'] = 0;
        }
        ans['data_begin_rotate'] = ans['data_rotate']
    },
    onrelease() {
        if(!CheckAns()) return;
        internal.add_cmd({
            data: {
                redo: ans.data_rotate,
                undo: ans.data_begin_rotate
            },
            redo(){RotateCallback(this.data.redo, true); sld_rotate.setValue(this.data.redo)},
            undo(){RotateCallback(this.data.undo, true); sld_rotate.setValue(this.data.undo); shell.print('[undo]', this.data.undo)}
        }, 'RotateCallback(' + ans.data_rotate + ', true)')
    }
})



ckbx_setting_rec_cmd = new gui.Checkbox({
    init_val: internal.enable_cmd_stk,
    text: 'Record Command',
    onclick: (boolean) => {
        internal.enable_cmd_stk = boolean
    }
})

ckbx_setting_rec_script = new gui.Checkbox({
    init_val: internal.enable_script_stk,
    text: 'Record Script',
    onclick: (boolean) => {
        internal.enable_script_stk = boolean
    }
})




// gui.ribbon.add(target button, Tab name, group name)

gui.ribbon.add(btn_read, 'Project', 'Files');
gui.ribbon.add(btn_outputScript, 'Project', 'Files');
gui.ribbon.add(btn_batchjob, 'Project', 'Files');

gui.ribbon.add(btn_save_jpg, 'Project', 'Save as');
gui.ribbon.add(btn_save_png, 'Project', 'Save as');
gui.ribbon.add(btn_save_bmp, 'Project', 'Save as');
gui.ribbon.add(btn_save_tif, 'Project', 'Save as');

gui.ribbon.add(btn_undo, 'Edit', 'Navigate');
gui.ribbon.add(btn_redo, 'Edit', 'Navigate');

gui.ribbon.add(btn_gray, 'Edit', 'Mono');
gui.ribbon.add(btn_bin, 'Edit', 'Mono');

gui.ribbon.addSlider(sld_rotate, 'Edit', 'Transform');

gui.ribbon.add(btn_equalize, 'Edit', 'Enhance');
gui.ribbon.add(btn_face, 'Edit', 'Enhance');

gui.ribbon.addWild(ckbx_setting_rec_cmd, 'Settings', 'Commands')
gui.ribbon.addWild(ckbx_setting_rec_script, 'Settings', 'Commands')
gui.ribbon.add(btn_open_devtools, 'Settings', 'Developer')

gui['Inspector'] = new gui.Window;
gui.Inspector.addWild(vw_global);
//gui.Inspector.add(btn_refresh_obj, 'Project', 'Object Viewer')
gui.Inspector.ribbon.add(btn_refresh_obj, 'Inspector', 'Refresh All')
eval(shell.readtxt('test-js/extensions/filters.js'))