shell["execfile"] = (filename) => {
    var src = shell.readtxt(filename);
    eval(src);
}