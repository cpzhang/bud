// enable double clicking from the Macintosh Finder or the Windows Explorer
#target photoshop

<javascriptresource>
<name>$$$/JavaScripts/OpenMPW/Menu=打开冰川mpw格式文件...</name>
</javascriptresource>
//
function read1(f)
{
    var a = parseInt(f.read (1).charCodeAt(0).toString())
    var v = a;
    return v;
}
function read2(f)
{
    var a = parseInt(f.read (1).charCodeAt(0).toString())
    var b = parseInt(f.read (1).charCodeAt(0).toString())
    var v = a + b * Math.pow(2, 8);
    return v;
}
function read4(f)
{
    var a = parseInt(f.read (1).charCodeAt(0).toString())
    var b = parseInt(f.read (1).charCodeAt(0).toString())
    var c = parseInt(f.read (1).charCodeAt(0).toString())
    var d = parseInt(f.read (1).charCodeAt(0).toString())
    var v = a + b * Math.pow(2, 8) + c * Math.pow(2, 16) + d * Math.pow(2, 24);
    return v;
}

function main()
{
    // in case we double clicked the file
    app.bringToFront();
    // alert(fconfig.fsName)
    var files = File.openDialog ("请选择mpw文件", "mpw files:*.mpw", true);
    if(files == null)
	return;
    var error;
    for(var i = 0; i != files.length; ++i)
    // if(file)
    {
	var f = files[i];
	var path = f.fsName;
	if(f.open("r"))
	{
	    f.encoding = "BINARY";
	    f.seek(0, 0);
	    f.seek(5*4, 0);
	    var extend = f.read (8);
	    if(extend.search("tga") >= 0)
	    {
		var totalFrames = read4(f);
		f.seek(2*4, 1);
		// var path = Folder.selectDialog("please select where to save this file");
		// if(path)
		{
		    var fc = new File(app.path + "\\Presets\\Scripts\\mpw\\config.ini");
		    fc.open("a");
		    for(var k = 0; k != totalFrames; ++k )
		    {
			var imageSize = read4(f);
			var imageWidth = read2(f);
			var imageHeight = read2(f);
			var offsetX = read2(f);
			var offsetY = read2(f);
			var tgaFilePath = app.path + "\\Presets\\Scripts\\mpw\\temple\\" + f.name.substr(0, f.name.length - 4) + "_" + k + "." + extend;
			// store and open tga image file
			var wf = new File(tgaFilePath);
			if(wf.open("w"))
			{
			    wf.encoding = "BINARY";
			    if(wf.write(f.read(imageSize)))
			    {
				wf.close();
			    }
			}
			app.open(wf);
			// insert tga file name into the config file
			fc.writeln(wf.fsName);
			var imageDataPos = f.tell() - imageSize;
			fc.writeln(imageDataPos);
			fc.writeln(imageSize);
		    }// for
		    fc.writeln(f.fsName);
		    // mark for another mpw file
		    fc.writeln("");
		    fc.close();
		}
	    }
	    else
	    {
		alert("mpw里面的图片不是tga格式，压缩后的mpw ？ ps 本身不支持dds格式, " + extend);
	    }
	    f.close()
	}
    }
}
// entry point
main();
