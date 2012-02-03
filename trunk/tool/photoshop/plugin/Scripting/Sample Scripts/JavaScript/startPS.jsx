// enable double clicking from the Macintosh Finder or the Windows Explorer
#target photoshop

function main()
{
    // in case we double clicked the file
    app.bringToFront();
    var f = new File(app.path + "\\Presets\\Scripts\\mpw\\config.ini");
    f.open("w");
    var d = new Date;
    f.writeln(d.toString());
    f.close();
}
// entry point
main();