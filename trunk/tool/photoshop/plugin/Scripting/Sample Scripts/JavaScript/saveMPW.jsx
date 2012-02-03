// enable double clicking from the Macintosh Finder or the Windows Explorer
#target photoshop

function main()
{
    //
    var fc = new File(app.path + "\\Presets\\Scripts\\mpw\\config.ini");
    fc.open("r");
    // date
    var d = fc.readln();
    // active document
    var ft = app.activeDocument.fullName;
    //
    var foundTGA = false;
    var mpwFilePath="";
    var frameNumber = 0;
    var imageDataPos;
    var imageSize;
    for(;;)
    {
	if(fc.eof == true)
	{
	    break;
	}
	var line = fc.readln();
	if(line == ft.fsName)
	{
	    foundTGA = true;
	    imageDataPos = parseInt(fc.readln());
	    imageSize = parseInt(fc.readln());
	}
	if(foundTGA)
	{
	    if(line == "")
	    {
		break;
	    }
	    ++frameNumber;
	    if(line.substr(line.length-4, 4) == ".mpw")
	    {
		mpwFilePath = line;
		break;
	    }
	}
    }
    //
    if(mpwFilePath != "")
    {
	var content;
	var f = new File(mpwFilePath);
	if(f.open("r"))
	{
	    f.encoding = "BINARY";
	    f.seek(0, 0);
	    content = f.read();
	    f.close();
	    f.remove();
	}
	var newContent0 = content.substr(0, imageDataPos);
	var newContent1;
	var newContent2 = content.substr(imageDataPos + imageSize, content.length - imageDataPos - imageSize);
	//
	ft.close();
	//
	f = new File(ft.fsName);
	if(f.open("r"))
	{
	    f.encoding = "BINARY";
	    f.seek(0, 0);
	    newContent1 = f.read();
	    f.close();
	}
	var newContent = newContent0 + newContent1 + newContent2;
	f = new File(mpwFilePath);
	if(f.open("w"))
	{
	    f.encoding = "BINARY";
	    f.write(newContent);
	    f.close();
	}
    }

    //
    fc.close();
}
// entry point
main();