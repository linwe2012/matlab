shell["execfile"] = function(filename) {
    var src = shell.readtxt(filename);
    eval(src);
}