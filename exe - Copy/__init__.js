shell["execfile"] = (filename) => {
    var src = shell.readtxt(filename);
    shell.pushCwd();
    shell.cd(shell.getDir(filename));
    eval(src);
    shell.popCwd();
}

shell["runtest"] = (name) => {
    if(shell.getExt(name).length == 0) {
        name = name + ".js"
    }

    shell.execfile('test-js/' + name)
}