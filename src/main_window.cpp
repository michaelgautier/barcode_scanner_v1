#include "main_window.hpp"

#include <iostream>

main_window::main_window()
{
    set_title ( "Gautier Scanner v01" );
    set_default_size ( 1920, 1080 );

    /*Menu*/
    HeaderActions = Gio::SimpleActionGroup::create();

    HeaderActions->add_action ( "configure", sigc::mem_fun ( *this, &main_window::on_configure ) );
    HeaderActions->add_action ( "export", sigc::mem_fun ( *this, &main_window::on_export ) );

    main_menu = Gio::Menu::create();
    main_menu->append ( "Configure", "win.configure" );
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

    set_focus ( barcode_field );

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
        guint text_length = barcode_buffer->get_length();

        if ( text_length > 5 )
        {
            barcode_row = barcode_record::create ( barcode_buffer->get_text() );

            bool found = false;
            bool limit_reached = false;
            const guint limit = scan_config_data.get_scan_config_items_per_container();

            guint barcode_count = barcode_list->get_n_items();

            for ( guint i = 0; i < barcode_count; i++ )
            {
                Glib::RefPtr<barcode_record> row = barcode_list->get_item ( i );

                found = row->value == barcode_row->value;

                if ( found )
                {
                    break;
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

                Gtk::Label* Description = Gtk::make_managed<Gtk::Label> ( Glib::ustring::format ( "Container - ", container_count ) );
                scanned_barcodes.append ( *Description );

                for ( guint i = 0; i < barcode_count; i++ )
                {
                    Glib::RefPtr<barcode_record> row = barcode_list->get_item ( i );

                    Description = Gtk::make_managed<Gtk::Label> ( row->value );

                    scanned_barcodes.append ( *Description );
                }

                barcode_list->remove_all();
            }
        }

        barcode_field.grab_focus();
    }

    return;
}

void main_window::on_export()
{
    std::cout << "Export\n";

    return;
}

void main_window::on_configure()
{
    scan_config_win = new scan_config_window;
    scan_config_win->signal_hide().connect ( sigc::mem_fun ( *this, &main_window::scan_config_on_close ) );
    scan_config_win->set_transient_for ( *this );
    scan_config_win->set_modal ( true );

    scan_config_win->set_config ( scan_config_data );
    scan_config_win->show();

    return;
}

void main_window::scan_config_on_close()
{
    /*
        Return values
            bool    scan_config_unique_barcodes = true;
            guint   scan_config_items_per_container = 10;
            guint   scan_config_barcode_length_min = 5;
            guint   scan_config_barcode_length_max = 15;
            ustring scan_config_container_prefix = "CONTAINER";
            ustring scan_config_container_suffix = "OUTBOUND";
            bool    scan_config_container_autoincrement = true;
            bool    scan_config_container_autoprint = false;
            guint   scan_config_export_format_typeid = 0;
    */

    scan_config_data = scan_config_win->get_config();

    delete scan_config_win;

    return;
}

