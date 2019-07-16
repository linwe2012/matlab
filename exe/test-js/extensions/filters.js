filter_registry = {
    sharpen: new Matrix,
    blur: new Matrix,
    detectEdge: new Matrix,
    boxBlur: new Matrix,
}

filter_registry.sharpen.fill([
    [-1, -1, -1],
    [-1, 9, -1],
    [-1, -1, -1]
])

/*
filter_registry.blur.fill([
    [1.0/16.0, 2.0/16.0, 1.0/16.0],
    [2/16.0, 4.0/16.0, 2.0/16.0],
    [1.0/16.0, 2.0/16.0, 1.0/16.0]
])*/

filter_registry.blur.fill([
    [1, 4, 6, 4, 1],
    [4, 16, 24, 16, 4],
    [6, 24, 36, 24, 6],
    [4, 16, 24, 16, 4],
    [1, 4, 6, 4, 1]
])

filter_registry.detectEdge.fill([
    [-1, -1, -1],
    [-1, 8, -1],
    [-1, -1, -1]
])

filter_registry.boxBlur.fill([
    [1, 1, 1, 1, 1],
    [1, 1, 1, 1, 1],
    [1, 1, 1, 1, 1],
    [1, 1, 1, 1, 1],
    [1, 1, 1, 1, 1]
])

filter_registry.boxBlur.divScale(1 / 25.0)

filter_registry.blur.divScale(1.0 / 7.1)

function FilterImage(name) {
    if(!CheckAns()) return;
    if(internal.enable_cmd_stk) {
        internal.add_cmd({
            data: {
                img: ans.clone(),
                name: name
            },
            redo(){FilterImage(this.data.name)},
            undo(){ans = this.data.img.clone(); RefreshImg(ans); shell.print('[undo]: Image filter')}
        }, 'FilterImage("' + name + '")')
    }
    

    ans.conv(filter_registry[name])
    RefreshImg(ans)
}

btn_sharpen = new gui.Button({
    text: 'Sharpen',
    icon: ':/icons/edit/sharpen',
    onclick: () => {
        FilterImage('sharpen')
    }
})

btn_blur = new gui.Button({
    text: 'Blur',
    icon: ':/icons/edit/blur',
    onclick: () => {
        FilterImage('blur')
    }
})

btn_detectEdge = new gui.Button({
    text: 'Detect Edge',
    icon: ':/icons/edit/radio-sig',
    onclick: () => {
        FilterImage('detectEdge')
    }
})

btn_boxBlur = new gui.Button({
    text: 'Box Blur',
    icon: ':/icons/edit/box',
    onclick: () => {
        FilterImage('boxBlur')
    }
})

gui.ribbon.add(btn_blur, 'Edit', 'Enhance')
gui.ribbon.add(btn_sharpen, 'Edit', 'Enhance')
gui.ribbon.add(btn_detectEdge, 'Edit', 'Tools')
gui.ribbon.add(btn_boxBlur, 'Edit', 'Tools')

