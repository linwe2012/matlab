var m1 = new Matrix;
m1.read('Lena.bmp');


shell.pushCwd();
shell.cd('test-output')

m1.write('copy.png');

{
    var m2 = m1.clone();
    m2.rotate(20);
    m2.write('rot_20.png');

    m2.rotate(-80);
    m2.write('rot_-60.png');

    m2.rotate(-10.5).rotate(-2.2);
    m2.write('rot_-72.7.png');
}


{
    var m2 = m1.clone();
    m2.togray().write('togray.png')
}


m1.clone().tobin().write('tobin.png')

m1.clone().equalizeHist().write('equalizeHist.png')

m1.clone().face().write('face.png');

shell.popCwd();