filter_registry = {
    sharpen: new Matrix,
    blur: new Matrix
}

filter_registry.sharpen.fill([
    [-1, -1, -1],
    [-1, 9, -1],
    [-1, -1, -1]
])

filter_registry.blur.fill([
    [1.0/16.0, 2.0/16.0, 1.0/16.0],
    [2/16.0, 4.0/16.0, 2.0/16.0],
    [1.0/16.0, 2.0/16.0, 1.0/16.0]
])

btn_sharpen = new gui.Button({
    text: 'Sharpen',
    icon: ':/icons/edit/sharpen',
    onclick: () => {
        if(!CheckAns()) return;
        ans.conv(filter_registry.sharpen)
        RefreshImg(ans)
    }
})

btn_blur = new gui.Button({
    text: 'Blur',
    icon: ':/icons/edit/blur',
    onclick: () => {
        if(!CheckAns()) return;
        ans.conv(filter_registry.blur)
        RefreshImg(ans)
    }
})

gui.ribbon.add(btn_blur, 'Edit', 'Enhance')
gui.ribbon.add(btn_sharpen, 'Edit', 'Enhance')