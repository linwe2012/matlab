shell["execfile"] = (filename) => {
    var src = shell.readtxt(filename);
    var m_cwd = shell.cwd;
    shell.cd(shell.getDir(filename));
    eval(src);
    shell.cwd = m_cwd;
}

shell["runtest"] = (name) => {
    if(shell.getExt(name).length == 0) {
        name = name + ".js"
    }

    shell.execfile('test-js/' + name)
}