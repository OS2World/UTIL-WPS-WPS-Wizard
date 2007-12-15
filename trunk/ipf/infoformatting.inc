.*infoformatting.inc: description of format options for info window
:h2 res=5200.Information window text formatting
:p.
The text of the folder information window may contain links and some basic formatting. The following
tags are currently supported. 
:nt.
Most tags must be closed at the end, this means text must be enclosed by two tags. 
:ent.

:parml.
:pt.:hp2.<h1>....</h1>:ehp2.
:pd.A heading.
:pt.:hp2.<a x>....</a>:ehp2.
:pd.A link. :hp2.x:ehp2. is a number which identifies the link.
:pt.:hp2.<br>:ehp2.
:pd.Line break.
:pt.:hp2.<p>...</p>:ehp2.
:pd.New paragraph
:pt.:hp2.<hr>:ehp2.
:pd.Ruler.
:pt.:hp2.<9 1>....<10>:ehp2.
:pd.New section with greater left margin.
:pt.:hp2.<font color="#aa00ff">....</font>:ehp2.
:pd.Change text color.
:pt.:hp2.<b>....</b>:ehp2.
:pd.Change text to bold.
:pt.:hp2.<gt>_</gt>:ehp2.
:pd.Insert :hp2.>:ehp2.. Make sure there's one character between the tags! 
:pt.:hp2.<lt>_</lt>:ehp2.
:pd.Insert :hp2.<:ehp2.. Make sure there's one character between the tags! 

:eparml.
:nt.
The tags introduced by V0.4.0 are still supported but deprecated. Use only the ones described above.
:ent.

