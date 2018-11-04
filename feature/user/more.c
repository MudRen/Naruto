// more.c
//#pragma save_binary

#include <ansi.h>

void more(string cmd, string *text, int line)
{
	int i, height;
	height = this_player()->query_temp("window/height") - 3;
	if( this_player()->query_temp("window/freeze") ) height -= 15;
	if( height <= 20 ) height = 20;
	switch(cmd) {
		case "b":
//		    line = (line >= 46) ? (line - 46) : 0;
//			write( "\r" + implode(text[line..line+22], "\n") + "\n" );
//			line += 23;
		    line = (line >= height*2) ? (line - height*2) : 0;
			write( "\r" + implode(text[line..line+height-1], "\n") + "\n" );
			line += height;
			break;
		case "q":
			return;
		default:
//	  	 	for(i=line + 23; line<sizeof(text) && line<i; line++)
	  	 	for(i=line + height; line<sizeof(text) && line<i; line++)
			write(text[line] + "\n");
//			if( line < 23 )
			if( line < height ) {
				write("\n");
				return;
			} 
			if( line >= sizeof(text) ) {
				write(HIW"-------------------------------------------[ �밴 Enter ������ ]------"NOR);
				input_to("pause");
				return;
			}
			break;
	}
	write(sprintf(HIW"== δ����� " NOR+YEL "��%d��/��%d��"GRN"(%d%%)"HIW" == (ENTER ������һҳ�oq �뿪�ob ǰһҳ)"NOR,
		line, sizeof(text), (line*100/sizeof(text)) ));
	input_to("more", text, line);
}

void start_more(string msg)
{
	more("", explode(msg, "\n"), 0);
}

void pause(string arg)
{
	write("\n");
	return;
}