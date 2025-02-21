#include "main_window.hpp"
#include <iostream>
#include <ctime>

main_window::main_window()
{
    set_title ( Glib::get_prgname() );
    set_default_size ( 800, 640 );
    set_decorated ( true );

    signal_show().connect ( sigc::bind ( sigc::mem_fun ( *this, &main_window::on_window_show ) ) );
    signal_destroy().connect ( sigc::bind ( sigc::mem_fun ( *this, &main_window::on_window_exit ) ) );

    return;
}

void main_window::on_window_show()
{
    /*Menu*/
    HeaderActions = Gio::SimpleActionGroup::create();

    HeaderActions->add_action ( "configure", sigc::mem_fun ( *this, &main_window::on_configure ) );
    HeaderActions->add_action ( "import", sigc::mem_fun ( *this, &main_window::on_import ) );
    HeaderActions->add_action ( "export", sigc::mem_fun ( *this, &main_window::on_export ) );

    main_menu = Gio::Menu::create();
    main_menu->append ( "Configure", "win.configure" );
    main_menu->append ( "Import", "win.import" );
    main_menu->append ( "Export", "win.export" );
    main_menu->freeze ( );

    main_menu_button.set_direction ( Gtk::ArrowType::NONE );
    main_menu_button.set_popover ( main_menu_popover );
    main_menu_popover.set_menu_model ( main_menu );

    main_header.pack_end ( main_menu_button );

    set_titlebar ( main_header );

    insert_action_group ( "win", HeaderActions );

    /*Layout*/
    main_frame.set_orientation ( Gtk::Orientation::HORIZONTAL );
    main_frame.set_start_child ( pane_left_frame );
    main_frame.set_end_child ( packages_scroller );

    input_frame.set_orientation ( Gtk::Orientation::HORIZONTAL );
    input_frame.append ( item_field_label );
    input_frame.append ( item_field );

    pane_left_frame.set_orientation ( Gtk::Orientation::VERTICAL );
    pane_left_frame.prepend ( input_frame );
    pane_left_frame.append ( items_in_progress_scroller );
    pane_left_frame.append ( input_actions_frame );

    input_actions_frame.set_end_widget ( input_actions_layout_frame );
    input_actions_layout_frame.set_orientation ( Gtk::Orientation::HORIZONTAL );
    input_actions_layout_frame.append ( reset_button );
    input_actions_layout_frame.append ( erase_all_button );

    packages_scroller.set_child ( packaged_items );
    items_in_progress_scroller.set_child ( items_in_progress );

    /*Styling*/
    add_css_class ( "app_baselevel" );

    main_menu_button.set_can_focus ( false );

    main_header.set_can_focus ( false );

    main_frame.set_shrink_start_child ( false );
    main_frame.set_resize_start_child ( false );
    main_frame.set_shrink_end_child ( false );
    main_frame.set_resize_end_child ( true );
    main_frame.set_position ( item_field.get_width() + 300 );

    item_field_label.set_margin ( 10 );
    item_field_label.set_text ( "M G 2025 9-e" );
    item_field_label.set_focus_on_click ( true );
    item_field_label.set_focusable ( true );
    item_field_label.add_css_class ( "scanner_label" );

    item_field.set_margin ( 10 );
    item_field.set_size_request ( 250 );
    item_field.set_halign ( Gtk::Align::START );
    item_field.set_valign ( Gtk::Align::CENTER );
    item_field.set_hexpand ( false );
    item_field.set_vexpand ( false );

    items_in_progress.set_halign ( Gtk::Align::FILL );
    items_in_progress.set_valign ( Gtk::Align::FILL );
    items_in_progress.set_hexpand ( true );
    items_in_progress.set_vexpand ( true );
    items_in_progress.set_show_row_separators ( true );
    items_in_progress.set_enable_rubberband ( true );
    items_in_progress.set_can_focus ( false );

    items_in_progress_scroller.set_margin ( 8 );
    items_in_progress_scroller.add_css_class ( "scroller" );

    input_actions_layout_frame.set_spacing ( 8 );

    packages_scroller.set_size_request ( 400 );
    packages_scroller.set_margin ( 8 );//Shortcut, set all margin sides
    packages_scroller.set_margin_bottom ( 60 );//Then override the bottom
    packages_scroller.add_css_class ( "scroller" );

    packaged_items.set_can_focus ( false );
    packaged_items.add_css_class ( "list_view" );

    reset_button.set_label ( "Reset" );
    reset_button.set_margin ( 8 );
    reset_button.set_can_focus ( false );
    reset_button.add_css_class ( "button_default" );

    erase_all_button.set_label ( "Erase all" );
    erase_all_button.set_margin ( 8 );
    erase_all_button.set_can_focus ( false );
    erase_all_button.add_css_class ( "button_default" );
    switch_erase_all_button_onoff();

    /*Tooltips*/
    item_field_label.set_tooltip_text ( "INPUT AN ITEM #\n-------\nScan barcode label or type in an item # then press ENTER" );
    input_actions_frame.set_tooltip_text ( "START OVER\n-------\nRESET clears single item field.\nERASE removes all in-progress items." );

    /*Item column visual data binding*/
    items_in_progress_signal_factory = Gtk::SignalListItemFactory::create();
    items_in_progress_signal_factory->signal_setup().connect ( sigc::bind ( sigc::mem_fun ( *this, &main_window::on_item_bind ) ) );
    items_in_progress_signal_factory->signal_bind().connect ( sigc::bind ( sigc::mem_fun ( *this, &main_window::on_item_bind ) ) );

    /*Item text entry ENTER key trigger*/
    item_field.signal_activate().connect ( sigc::bind ( sigc::mem_fun ( *this, &main_window::on_item_activate ) ) );

    /*Reset button signal*/
    reset_button.signal_clicked().connect ( sigc::bind ( sigc::mem_fun ( *this, &main_window::on_reset_clicked ) ) );

    /*Erase all button signal*/
    erase_all_button.signal_clicked().connect ( sigc::bind ( sigc::mem_fun ( *this, &main_window::on_erase_all_clicked ) ) );

    /*This has to occur last so binding occurs correctly.*/
    /*Items in Progress*/
    //1.) Need a data list to hold scanned item values.
    item_list = Gio::ListStore<barcode_record>::create();
    //2.) Wrap the data list in an interface used by visual list widgets.
    Glib::RefPtr<Gtk::SingleSelection> item_model = Gtk::SingleSelection::create ( item_list );
    //3.) Link the data list to the visual list widget.
    items_in_progress.set_model ( item_model );
    //4.) Define a view to visualize one or more values from the model.
    item_column = Gtk::ColumnViewColumn::create ( "ITEMS in progress", items_in_progress_signal_factory );
    //5.) Link the item(s) visual to the aggregate visual.
    items_in_progress.append_column ( item_column );

    /*Column has to be styled separately from the other visuals since its creation is delayed until the end.*/
    item_column->set_expand ( true );
    item_column->set_resizable ( false );

    set_child ( main_frame );
    set_focus ( item_field );

    return;
}

void main_window::on_window_exit()
{
    if ( item_column )
    {
        item_column = nullptr;
    }

    if ( item_row )
    {
        item_row = nullptr;
    }

    if ( item_list )
    {
        item_list->remove_all();
        item_list = nullptr;
    }

    if ( HeaderActions )
    {
        HeaderActions = nullptr;
    }

    if ( main_menu )
    {
        main_menu->remove_all();
        main_menu = nullptr;
    }

    if ( FileOpenOperationDialog )
    {
        FileOpenOperationDialog = nullptr;
    }

    if ( FileSaveOperationDialog )
    {
        FileSaveOperationDialog = nullptr;
    }

    if ( items_in_progress_signal_factory )
    {
        items_in_progress_signal_factory = nullptr;
    }

    if ( scan_config_win )
    {
        scan_config_win = nullptr;
    }

    return;
}

void main_window::on_item_bind ( const Glib::RefPtr<Gtk::ListItem>& list_item )
{
    auto row = std::dynamic_pointer_cast<barcode_record> ( list_item->get_item() );

    if ( !row )
    {
        if ( list_item )
        {
            list_item->set_child ( *Gtk::make_managed<Gtk::Label> ( "" ) );
        }

        return;
    }
    else
    {
        auto label = dynamic_cast<Gtk::Label*> ( list_item->get_child() );

        if ( !label )
        {
            return;
        }

        label->add_css_class ( "list_item" );
        label->set_text ( row->value );
    }

    return;
}

void main_window::on_item_activate()
{
    Glib::RefPtr<Gtk::EntryBuffer> item_buffer = item_field.get_buffer();

    if ( item_buffer )
    {
        const guint text_length = item_buffer->get_length();

        const guint item_length_min = scan_config_data.get_scan_config_item_length_min();
        const guint item_length_max = scan_config_data.get_scan_config_item_length_max();

        if ( text_length >= item_length_min && text_length <= item_length_max )
        {
            item_row = barcode_record::create ( item_buffer->get_text() );

            bool found = false;
            bool limit_reached = false;
            const guint limit = scan_config_data.get_scan_config_items_per_container();

            guint item_count = item_list->get_n_items();

            const bool unique_scan_required = scan_config_data.get_scan_config_unique_items();

            for ( guint i = 0; i < item_count; i++ )
            {
                Glib::RefPtr<barcode_record> row = item_list->get_item ( i );

                if ( unique_scan_required )
                {
                    found = row->value == item_row->value;

                    if ( found )
                    {
                        break;
                    }
                }
            }

            if ( found == false )
            {
                item_list->append ( item_row );

                item_field.set_text ( "" );

                item_count = item_list->get_n_items();

                limit_reached = limit == item_count;
            }

            if ( limit_reached )
            {
                package_count++;

                Glib::ustring LabelPrefix = scan_config_data.get_scan_config_container_prefix();
                Glib::ustring LabelSuffix = scan_config_data.get_scan_config_container_suffix();
                Glib::ustring AutoIncrementText;

                if ( scan_config_data.get_scan_config_container_autoincrement() )
                {
                    AutoIncrementText = Glib::ustring::format ( package_count );
                }

                Glib::ustring container_name = Glib::ustring::format ( LabelPrefix, " ", AutoIncrementText, " ", LabelSuffix );

                Gtk::Label* item_description = Gtk::make_managed<Gtk::Label> ( container_name );
                packaged_items.append ( *item_description );

                for ( guint i = 0; i < item_count; i++ )
                {
                    Glib::RefPtr<barcode_record> row = item_list->get_item ( i );

                    item_description = Gtk::make_managed<Gtk::Label> ( row->value );

                    packaged_items.append ( *item_description );
                }

                item_list->remove_all();

                package_item_count += item_count;

                if ( package_item_count > 23 )
                {
                    packaged_items.set_tooltip_text ( Glib::ustring::format ( "QUICK LOOK\n-------\n", "Most recent package: ", container_name, "\n Total packages: ", package_count, "\n Total items: ",
                                                      package_item_count ) );
                }

                if ( scan_config_data.get_scan_config_container_autoprint() )
                {
                    std::cout << "Label Print\n";
                }
            }
        }

        switch_erase_all_button_onoff();
        item_field.grab_focus();
    }

    return;
}

void main_window::on_import()
{
    Glib::RefPtr<Gtk::FileFilter> AllowedFileType = Gtk::FileFilter::create();
    AllowedFileType->add_suffix ( "txt" );

    FileOpenOperationDialog = Gtk::FileChooserNative::create ( "File Import", Gtk::FileChooser::Action::OPEN );
    FileOpenOperationDialog->set_current_folder ( Gio::File::create_for_path ( Glib::get_home_dir () ) );
    FileOpenOperationDialog->signal_response().connect ( sigc::mem_fun ( *this, &main_window::file_open_response ) );
    FileOpenOperationDialog->set_create_folders ( false );
    FileOpenOperationDialog->set_select_multiple ( false );
    FileOpenOperationDialog->set_filter ( AllowedFileType );
    FileOpenOperationDialog->set_modal ( true );
    FileOpenOperationDialog->set_transient_for ( *this );
    FileOpenOperationDialog->show();

    return;
}

void main_window::file_open_response ( int response_id )
{
    if ( response_id == Gtk::ResponseType::ACCEPT )
    {
        Glib::RefPtr<Gio::File> file_ref = FileOpenOperationDialog->get_file();

        if ( file_ref->query_exists() )
        {
            std::cout << "Values input file path " << file_ref->get_path() << "\n";

            Glib::RefPtr<Gio::FileInputStream> file_io_ref = file_ref->read();
            Glib::RefPtr<Gio::DataInputStream> file_lines_ref = Gio::DataInputStream::create ( file_io_ref );

            bool reading_lines = true;
            std::string line_data;

            while ( reading_lines )
            {
                reading_lines = file_lines_ref->read_line ( line_data );

                if ( line_data.empty() == false )
                {
                    item_field.set_text ( line_data );
                    on_item_activate();

                    line_data.clear();
                }
            }

            if ( file_io_ref->is_closed() == false )
            {
                file_io_ref->close();
                file_io_ref = nullptr;
            }
        }
    }
    else if ( response_id == Gtk::ResponseType::CANCEL )
    {
        std::cout << "Cancelled file import\n";
    }
    else if ( response_id == Gtk::ResponseType::DELETE_EVENT )
    {
        std::cout << "Abrupt file import dialog closure\n";
    }

    FileOpenOperationDialog = nullptr;

    return;
}

void main_window::on_export()
{
    if ( package_count > 0 && package_item_count > 0 )
    {
        FileSaveOperationDialog = Gtk::FileChooserNative::create ( "File Export", Gtk::FileChooser::Action::SELECT_FOLDER );
        FileSaveOperationDialog->set_current_folder ( Gio::File::create_for_path ( Glib::get_home_dir () ) );
        FileSaveOperationDialog->signal_response().connect ( sigc::mem_fun ( *this, &main_window::file_save_response ) );
        FileSaveOperationDialog->set_create_folders ( true );
        FileSaveOperationDialog->set_modal ( true );
        FileSaveOperationDialog->set_transient_for ( *this );
        FileSaveOperationDialog->show();
    }

    return;
}

void main_window::file_save_response ( int response_id )
{
    if ( response_id == Gtk::ResponseType::ACCEPT )
    {
        Glib::RefPtr<Gio::File> folder_ref = FileSaveOperationDialog->get_current_folder();

        std::string directory_path = folder_ref->get_path();

        std::cout << "exporting to folder path " << directory_path << "\n";

        std::string file_name;

        switch ( scan_config_data.get_scan_config_export_format_type() )
        {
            case scan_config_export_type::tab_delimited:
                file_name = "export.tsv";
                break;

            case scan_config_export_type::xml:
                file_name = "export.xml";
                break;

            case scan_config_export_type::edi_856:
                file_name = "export.edi856";
                break;

            default:
                file_name = "export.txt";
                break;
        }

        std::string file_path = Glib::build_filename ( directory_path, file_name );

        Glib::RefPtr<Gio::File> file_ref = Gio::File::create_for_path ( file_path );

        Glib::RefPtr<Gio::FileOutputStream> file_io_ref;

        std::cout << "Values output file path " << file_ref->get_path() << "\n";

        if ( file_ref->query_exists() )
        {
            file_io_ref = file_ref->replace ( "", true, Gio::File::CreateFlags::REPLACE_DESTINATION );
        }
        else
        {
            file_io_ref = file_ref->create_file ( Gio::File::CreateFlags::PRIVATE );
        }

        Glib::RefPtr<Gio::DataOutputStream> file_lines_ref = Gio::DataOutputStream::create ( file_io_ref );

        int row_num = 0;
        int total_items = 0;
        int edi856_st_line_count = 0;
        int edi856_line_item_count = 0;
        Gtk::ListBoxRow* container_row = packaged_items.get_row_at_index ( row_num++ );

        std::string row_label_text;
        std::string container_name;

        const scan_config_export_type export_type = scan_config_data.get_scan_config_export_format_type();

        if ( export_type == scan_config_export_type::xml )
        {
            file_lines_ref->put_string ( "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n" );
            file_lines_ref->put_string ( "<document>\n" );
        }
        else if ( export_type == scan_config_export_type::edi_856 )
        {
            const std::time_t ctime = std::time ( nullptr );
            std::tm* ctime_v = std::localtime ( &ctime );

            Glib::ustring date_text = Glib::ustring::format ( ctime_v->tm_year, ctime_v->tm_mon, ctime_v->tm_mday );
            Glib::ustring time_text = Glib::ustring::format ( ctime_v->tm_hour, ctime_v->tm_min );

            file_lines_ref->put_string ( "ISA*00**00**ZZ*GAUTIER*99*WORLDWIDE*8738473*1050*U*00401*23847988*0*P*>~\n" );
            file_lines_ref->put_string ( "GS*SH*GAUTIER*WORLDWIDE*" + date_text + "*1050*23847988*X*004010~\n" );
            file_lines_ref->put_string ( "ST*856*" + time_text + "~\n" );
            file_lines_ref->put_string ( "BSN*00*SHIPMENTNUMBER*" + date_text + "*HHMM~\n" );
            file_lines_ref->put_string ( "DTM*002*" + date_text + "~\n" );
            file_lines_ref->put_string ( "HL*1**S~\n" );

            edi856_st_line_count = 6;
        }
        else
        {
            file_lines_ref->put_string ( "\n" );
        }

        while ( container_row )
        {
            Gtk::Label* row_label = dynamic_cast<Gtk::Label*> ( container_row->get_child() );

            if ( row_label )
            {
                row_label_text = row_label->get_text();

                bool has_prefix = Glib::str_has_prefix ( row_label_text, scan_config_data.get_scan_config_container_prefix() );
                bool has_suffix = Glib::str_has_suffix ( row_label_text, scan_config_data.get_scan_config_container_suffix() );
                bool close_line = false;

                if ( has_prefix || has_suffix )
                {
                    if ( container_name.empty() == false )
                    {
                        close_line = true;
                    }

                    container_name = row_label_text;
                    row_label_text.clear();
                }
                else
                {
                    total_items++;
                }

                switch ( export_type )
                {
                    case scan_config_export_type::tab_delimited:
                        if ( close_line )
                        {
                            file_lines_ref->put_string ( "\n" );
                        }

                        write_tab_delimited_line ( file_lines_ref, container_name, row_label_text );
                        break;

                    case scan_config_export_type::xml:
                        if ( close_line )
                        {
                            close_xml_line ( file_lines_ref );
                        }

                        write_xml_line ( file_lines_ref, container_name, row_label_text );
                        break;

                    case scan_config_export_type::edi_856:
                        {
                            const int written_line_count = write_edi_856_line ( file_lines_ref, total_items, scan_config_data.get_scan_config_container_suffix(), row_label_text );
                            edi856_line_item_count += written_line_count;
                            edi856_st_line_count += written_line_count;
                        }
                        break;

                    default:
                        if ( close_line )
                        {
                            file_lines_ref->put_string ( "\n" );
                        }

                        write_text_line ( file_lines_ref, row_label_text );
                        break;
                }

                row_label_text.clear();
            }

            container_row = packaged_items.get_row_at_index ( row_num++ );
        }

        if ( container_name.empty() == false && export_type == scan_config_export_type::xml )
        {
            file_lines_ref->put_string ( "\n\t\t</line>\n" );
            file_lines_ref->put_string ( "\t</group>\n" );
            file_lines_ref->put_string ( "</document>" );
        }
        else if ( export_type == scan_config_export_type::edi_856 )
        {
            file_lines_ref->put_string ( Glib::ustring::format ( "CTT*1*", edi856_line_item_count, "~\n" ) );
            file_lines_ref->put_string ( Glib::ustring::format ( "SE*1*", edi856_st_line_count, "~\n" ) );
            file_lines_ref->put_string ( "GE*1*23847988~\n" );
            file_lines_ref->put_string ( "IEA*1*23847988~\n" );
        }
        else
        {
            file_lines_ref->put_string ( "\n" );
        }

        file_io_ref->flush();

        if ( file_io_ref->is_closed() == false )
        {
            file_io_ref->close();
            file_io_ref = nullptr;
        }
    }
    else if ( response_id == Gtk::ResponseType::CANCEL )
    {
        std::cout << "Cancelled file export\n";
    }
    else if ( response_id == Gtk::ResponseType::DELETE_EVENT )
    {
        std::cout << "Abrupt file export dialog closure\n";
    }

    FileSaveOperationDialog = nullptr;

    return;
}

void main_window::write_tab_delimited_line ( Glib::RefPtr<Gio::DataOutputStream> out, const std::string container_name, const std::string value )
{
    if ( value.empty() && container_name.empty() == false )
    {
        out->put_string ( container_name );
    }
    else if ( value.empty() == false )
    {
        out->put_string ( "\t" + value );
    }

    return;
}

void main_window::write_xml_line ( Glib::RefPtr<Gio::DataOutputStream> out, const std::string container_name, const std::string value )
{
    if ( value.empty() && container_name.empty() == false )
    {
        out->put_string ( "\t<group name=\"" );
        out->put_string ( container_name );
        out->put_string ( "\">\n" );
        out->put_string ( "\t\t<line>\n" );
    }
    else if ( value.empty() == false )
    {
        out->put_string ( "\t\t\t<item value=\"" );
        out->put_string ( value );
        out->put_string ( "\" />\n" );
    }

    return;
}

void main_window::close_xml_line ( Glib::RefPtr<Gio::DataOutputStream> out )
{
    out->put_string ( "\n\t\t</line>\n" );
    out->put_string ( "\t</group>\n" );

    return;
}

int main_window::write_edi_856_line ( Glib::RefPtr<Gio::DataOutputStream> out, const int line_no, const std::string batch_number, const std::string value )
{
    int line_count = 0;

    if ( value.empty() == false )
    {
        std::string line_no_str = Glib::ustring::format ( line_no );
        std::string line_text;

        const int line_no_ubound = 5;
        int line_no_left_pad_count = line_no_ubound - line_no_str.size();
        std::string line_no_pad;

        line_no_pad.append ( line_no_left_pad_count, '0' );
        line_no_pad.append ( line_no_str );

        out->put_string ( "LIN*" + line_no_pad + "*PN*8-002811*VP*8-002811~\n" );
        out->put_string ( "SN1*" + line_no_pad + "*1*EA****ITM*" + batch_number + "*SN*" + value + "~\n" );

        line_count = 2;
    }

    return line_count;
}

void main_window::write_text_line ( Glib::RefPtr<Gio::DataOutputStream> out, const std::string value )
{
    out->put_string ( value + "\n" );

    return;
}

void main_window::on_configure()
{
    scan_config_win = new scan_config_window;
    scan_config_win->set_hide_on_close ( true );
    scan_config_win->signal_hide().connect ( sigc::mem_fun ( *this, &main_window::scan_config_on_close ) );
    scan_config_win->set_transient_for ( *this );
    scan_config_win->set_modal ( true );

    scan_config_win->set_config ( scan_config_data );
    scan_config_win->show();

    return;
}

void main_window::scan_config_on_close()
{
    scan_config_data = scan_config_win->get_config();

    item_field.set_max_length ( scan_config_data.get_scan_config_item_length_max() );

    delete scan_config_win;

    return;
}

void main_window::on_reset_clicked()
{
    item_field.set_text ( "" );
    set_focus ( item_field );

    return;
}

void main_window::on_erase_all_clicked()
{
    on_reset_clicked();
    item_list->remove_all();

    switch_erase_all_button_onoff();

    return;
}

bool main_window::switch_erase_all_button_onoff()
{
    const int count = ( item_list ) ? item_list->get_n_items() : 0;
    const bool has_items = count > 0;

    erase_all_button.set_sensitive ( has_items );

    return erase_all_button.get_sensitive();
}
