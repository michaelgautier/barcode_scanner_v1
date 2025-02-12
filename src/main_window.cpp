#include "main_window.hpp"
#include <iostream>

main_window::main_window()
{
    set_title ( "Gautier Scanner v01" );
    set_default_size ( 1920, 1080 );

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
    main_frame.prepend ( left_frame );
    main_frame.append ( scanned_barcodes_scroller );

    input_frame.set_orientation ( Gtk::Orientation::HORIZONTAL );
    input_frame.append ( barcode_field_label );
    input_frame.append ( barcode_field );

    left_frame.set_orientation ( Gtk::Orientation::VERTICAL );
    left_frame.prepend ( input_frame );
    left_frame.append ( barcodes_scroller );
    left_frame.append ( scan_options_center_frame );

    scan_options_center_frame.set_end_widget ( scan_options_frame );
    scan_options_frame.append ( scan_reset_button );
    scan_options_frame.append ( scan_erase_all_button );

    scan_options_frame.set_orientation ( Gtk::Orientation::HORIZONTAL );
    scan_options_frame.set_spacing ( 8 );
    scan_reset_button.set_label ( "Reset" );
    scan_erase_all_button.set_label ( "Erase all" );

    scanned_barcodes_scroller.set_child ( scanned_barcodes );
    scanned_barcodes_scroller.set_size_request ( 1100 );

    set_child ( main_frame );

    /*Barcode Scanning*/
    barcode_field_label.set_margin ( 10 );
    barcode_field_label.set_text ( "barcode" );

    barcode_field.set_margin ( 10 );
    barcode_field.set_size_request ( 400 );
    barcode_field.set_halign ( Gtk::Align::START );
    barcode_field.set_valign ( Gtk::Align::CENTER );
    barcode_field.set_hexpand ( false );
    barcode_field.set_vexpand ( false );

    barcodes_scroller.set_child ( barcodes );

    barcodes.set_halign ( Gtk::Align::FILL );
    barcodes.set_valign ( Gtk::Align::FILL );
    barcodes.set_hexpand ( true );
    barcodes.set_vexpand ( true );

    /*Barcode accumulation storage*/
    container_count = 0;

    barcode_list = Gio::ListStore<barcode_record>::create();

    /*Bind barcode storage to barcode list visual*/
    auto barcode_model = Gtk::SingleSelection::create ( barcode_list );
    barcodes.set_model ( barcode_model );

    /*Barcode column visual data binding*/
    barcode_list_signal_factory = Gtk::SignalListItemFactory::create();
    barcode_list_signal_factory->signal_setup().connect ( sigc::bind ( sigc::mem_fun ( *this, &main_window::on_barcode_bind ) ) );
    barcode_list_signal_factory->signal_bind().connect ( sigc::bind ( sigc::mem_fun ( *this, &main_window::on_barcode_bind ) ) );

    /*Barcode column visuals*/
    barcode_column = Gtk::ColumnViewColumn::create ( "Barcodes", barcode_list_signal_factory );

    barcodes.append_column ( barcode_column );

    /*Barcode text entry ENTER key trigger*/
    barcode_field.signal_activate().connect ( sigc::bind ( sigc::mem_fun ( *this, &main_window::on_barcode_activate ) ) );

    /*Reset button signal*/
    scan_reset_button.signal_clicked().connect ( sigc::bind ( sigc::mem_fun ( *this, &main_window::on_reset_clicked ) ) );

    /*Erase all button signal*/
    scan_erase_all_button.signal_clicked().connect ( sigc::bind ( sigc::mem_fun ( *this, &main_window::on_erase_all_clicked ) ) );

    set_focus ( barcode_field );

    return;
}

void main_window::on_window_exit()
{
std::cout << "exit main_window\n";
    if ( barcode_column )
    {
        barcode_column = nullptr;
    }

    if ( barcode_row )
    {
        barcode_row = nullptr;
    }

    if ( barcode_list )
    {
    	barcode_list->remove_all();
        barcode_list = nullptr;
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

    if ( barcode_list_signal_factory )
    {
        barcode_list_signal_factory = nullptr;
    }

    if ( scan_config_win )
    {
        scan_config_win = nullptr;
    }

    return;
}

void main_window::on_barcode_bind ( const Glib::RefPtr<Gtk::ListItem>& list_item )
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

        label->set_text ( row->value );
    }

    return;
}

void main_window::on_barcode_activate()
{
    Glib::RefPtr<Gtk::EntryBuffer> barcode_buffer = barcode_field.get_buffer();

    if ( barcode_buffer )
    {
        const guint text_length = barcode_buffer->get_length();

        const guint barcode_length_min = scan_config_data.get_scan_config_barcode_length_min();
        const guint barcode_length_max = scan_config_data.get_scan_config_barcode_length_max();

        if ( text_length >= barcode_length_min && text_length <= barcode_length_max )
        {
            barcode_row = barcode_record::create ( barcode_buffer->get_text() );

            bool found = false;
            bool limit_reached = false;
            const guint limit = scan_config_data.get_scan_config_items_per_container();

            guint barcode_count = barcode_list->get_n_items();

            const bool unique_scan_required = scan_config_data.get_scan_config_unique_barcodes();

            for ( guint i = 0; i < barcode_count; i++ )
            {
                Glib::RefPtr<barcode_record> row = barcode_list->get_item ( i );

                if ( unique_scan_required )
                {
                    found = row->value == barcode_row->value;

                    if ( found )
                    {
                        break;
                    }
                }
            }

            if ( !found )
            {
                barcode_list->append ( barcode_row );

                barcode_field.set_text ( "" );

                barcode_count = barcode_list->get_n_items();

                limit_reached = limit == barcode_count;
            }

            if ( limit_reached )
            {
                container_count++;

                Glib::ustring LabelPrefix = scan_config_data.get_scan_config_container_prefix();
                Glib::ustring LabelSuffix = scan_config_data.get_scan_config_container_suffix();
                Glib::ustring AutoIncrementText;

                if ( scan_config_data.get_scan_config_container_autoincrement() )
                {
                    AutoIncrementText = Glib::ustring::format ( container_count );
                }

                Gtk::Label* Description = Gtk::make_managed<Gtk::Label> ( Glib::ustring::format ( LabelPrefix, " ", AutoIncrementText, " ", LabelSuffix ) );
                scanned_barcodes.append ( *Description );

                for ( guint i = 0; i < barcode_count; i++ )
                {
                    Glib::RefPtr<barcode_record> row = barcode_list->get_item ( i );

                    Description = Gtk::make_managed<Gtk::Label> ( row->value );

                    scanned_barcodes.append ( *Description );
                }

                barcode_list->remove_all();

                if ( scan_config_data.get_scan_config_container_autoprint() )
                {
                    std::cout << "Label Print\n";
                }
            }
        }

        barcode_field.grab_focus();
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
                    barcode_field.set_text ( line_data );
                    on_barcode_activate();

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
    FileSaveOperationDialog = Gtk::FileChooserNative::create ( "File Export", Gtk::FileChooser::Action::SELECT_FOLDER );
    FileSaveOperationDialog->set_current_folder ( Gio::File::create_for_path ( Glib::get_home_dir () ) );
    FileSaveOperationDialog->signal_response().connect ( sigc::mem_fun ( *this, &main_window::file_save_response ) );
    FileSaveOperationDialog->set_create_folders ( true );
    FileSaveOperationDialog->set_modal ( true );
    FileSaveOperationDialog->set_transient_for ( *this );
    FileSaveOperationDialog->show();

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
        Gtk::ListBoxRow* container_row = scanned_barcodes.get_row_at_index ( row_num++ );

        std::string row_label_text;

        while ( container_row )
        {
            Gtk::Label* row_label = dynamic_cast<Gtk::Label*> ( container_row->get_child() );

            if ( row_label )
            {
                row_label_text = row_label->get_text();

                file_lines_ref->put_string ( row_label_text + "\n" );

                row_label_text.clear();
            }

            container_row = scanned_barcodes.get_row_at_index ( row_num++ );
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

    delete scan_config_win;

    return;
}

void main_window::on_reset_clicked()
{
    barcode_field.set_text ( "" );
    set_focus ( barcode_field );

    return;
}

void main_window::on_erase_all_clicked()
{
    on_reset_clicked();
    barcode_list->remove_all();

    return;
}
