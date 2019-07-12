ans = null;

btn_bin = new gui.Button(
    {
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
        text: 'Mono Color',
        onclick: ()=> {
            if(!CheckAns()) return;
            ans.togray();
            gui.display(ans)
        }
    }
);

btn_read = new gui.Button(
    {
        text: 'Open...',
        onclick: ()=> {
            ans = new Matrix;
            ans.read('test-js/Lena.bmp')
            gui.display(ans)
        }
    }
)


function CheckAns() {
    if(ans == null) {
        console.log('Image is not loaded');
        return false;
    }
    return true;
}
