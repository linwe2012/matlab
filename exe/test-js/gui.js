var ans = null;

function CheckAns() {
    if(ans == null) {
        console.log('Image is not loaded');
        return false;
    }
    return true;
}

var bin = new gui.Button(
    {
        text: 'Binerize',
        icon: "????",
        // click callback
        onclick: ()=> {
            if(!CheckAns()) return;

            ans.tobin();
        }
    }
)


function RotateCallback(degree) {
    if(!CheckAns()) return;

    if(ans['rotate'] === undefined || ans['rotate'] === null) {
        ans['rotate'] = 0;
    }

    var delta = degree - ans['rotate'];
    ans.rotate(delta);
}

var rotate = new gui.Silder (
    {
        max: 360,
        min: 0,
        text: 'Rotate',
        onclick: RotateCallback
    }
)

var paint = new gui.DragTool (
    {
        onbegin: (x, y)=>{},
        ondrag: (x, y)=>{},
        onend: (x, y)=>{}
    }
)

gui.Toolbar.add(bin);
gui.Toolbar.add(rotate);
