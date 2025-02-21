#include "application.hpp"

gautier_application::gautier_application()
{
    Glib::set_prgname ( "Gautier_Scanner_v1.F" );
    Glib::set_application_name ( "Gautier Scanner" );

    set_id ( "gautier.scanner.app_v01f" );
    set_flags ( Gio::Application::Flags::NON_UNIQUE );

    return;
}

void gautier_application::on_startup()
{
    Gtk::Application::on_startup();

    /*Styling*/
    css = Gtk::CssProvider::create();

    Gtk::StyleContext::add_provider_for_display ( Gdk::Display::get_default(), css, GTK_STYLE_PROVIDER_PRIORITY_USER );

    std::string style_data =
        R"(
	.app_baselevel 
	{ 
		background: rgb(70,30,8);
		background: linear-gradient(22deg, rgba(70,30,8,1) 12%, rgba(166,164,38,1) 51%, rgba(70,30,8,1) 90%);
	}

	.config_background 
	{ 
		background: #665a22;
	}

	.scanner_label
	{
		font-family: "Libre Barcode 39 Extended Text", "monospace";
		font-size: 16pt;
		font-weight: normal;
		color: #502426;
	}

	.standard_label
	{
		font-family: "monospace";
		font-size: 8pt;
		font-weight: normal;
		color: #f6deb5;
	}

	.section_label
	{
		font-family: "monospace";
		font-size: 16pt;
		font-weight: bold;
		color: #f6ebb5;
	}

	.button_default
	{
		background: rgb(249,159,109);
		background: linear-gradient(0deg, rgba(249,159,109,1) 12%, rgba(74,73,5,1) 37%, rgba(113,94,68,1) 66%, rgba(255,185,146,1) 90%);
		color: orange;
	}

	.button_default:hover
	{
		color: #c8fefb;
	}

	.button_default:disabled
	{
		background: rgb(213,200,192);
		background: linear-gradient(0deg, rgba(213,200,192,1) 12%, rgba(125,125,105,1) 37%, rgba(129,116,99,1) 66%, rgba(190,172,162,1) 90%); 
		color: white;
	}

	.toggle_button
	{
		background: #264350;
		color: orange;
	}

	.toggle_button:hover
	{
		color: #c8fefb;
	}

	.toggle_button:checked
	{
		background: #e7cf26;
		color: #22525b;
	}

	.toggle_button:disabled
	{
		background: #6b7f88;
		color: white;
	}
	
	.scroller
	{
		background: none;
		border-color: #ffcc00ff;
		border-style: solid;
		border-width: 2px;
	}

	.list_view
	{
		background: rgb(28,75,90);
		background: linear-gradient(172deg, rgba(28,75,90,1) 0%, rgba(90,28,28,1) 28%, rgba(80,22,22,1) 66%, rgba(68,22,80,1) 100%);
		color: orange;
		font-family: "monospace";
		font-weight: normal;
		font-size: 14pt;
	}

	.list_item
	{
		background: none;
	}
	
	/*
	***** GTK Overrides
	********** Override the default style for certain elements in GTK where typical directives in css class names are not recognized or applied.
	********** Sometimes the scope or reach of a typical css selector does not extend into intermediate elements.
	********** Use Ctrl + Shift + D in the GTK application while it is running to inspect the layout in real-time.
	*/

	/*
	---- listview: The css addressable name for the actual widget that is dynamically created to host ListItems.
	*/
	columnview > header > button
	{
		background-color: #1c4b5aff;
		border-color: #8c2a13;
		border-style: dashed;
		border-width: 2px;
		color: orange;
	}

	columnview > listview
	{
		background: rgb(28,75,90);
		background: linear-gradient(172deg, rgba(28,75,90,1) 0%, rgba(90,28,28,1) 28%, rgba(80,22,22,1) 66%, rgba(68,22,80,1) 100%);
		color: orange;
	}

	columnview > listview > row:selected
	{
		background-color: #df9e31;
		border-color: #f9edfa;
		border-style: dashed;
		border-width: 1px;
		color: #234378;
	}

	.list_view > row:selected
	{
		background-color: #df9e31;
		border-color: #f9edfa;
		border-style: dashed;
		border-width: 1px;
		color: #234378;
		font-family: "monospace";
		font-weight: normal;
		font-size: 14pt;
	}
    )";

    css->load_from_data ( style_data );

    return;
}
