#ifndef inc_gautier_ui_main_window
#define inc_gautier_ui_main_window

#include "config.h"
#include <gtkmm.h>
#include <glibmm.h>

#include "barcode_record.hpp"
#include "scan_config_record.hpp"
#include "scan_config_window.hpp"

class main_window : public Gtk::ApplicationWindow {
    public:
        main_window();
    private:
        void on_window_show();
        void on_window_exit();

        //Header
        Glib::RefPtr<Gio::Menu> main_menu;

        Gtk::PopoverMenu main_menu_popover;
        Gtk::HeaderBar main_header;
        Gtk::MenuButton main_menu_button;

        //Header Menu Actions
        Glib::RefPtr<Gio::SimpleActionGroup> HeaderActions;

        //Contains all visuals
        Gtk::Paned main_frame;

        //Item input
        Gtk::Box pane_left_frame;
        Gtk::Box input_frame; //Contains the input fields
        Gtk::Label item_field_label;
        Gtk::Entry item_field;

        //Items In Progress - Listing
        Gtk::ScrolledWindow items_in_progress_scroller;
        Gtk::ColumnView items_in_progress;
        Glib::RefPtr<Gtk::ColumnViewColumn> item_column;

        //Items In Progress - Buttons
        Gtk::Box input_actions_layout_frame;
        Gtk::CenterBox input_actions_frame;
        Gtk::Button reset_button;
        Gtk::Button erase_all_button;

        void on_reset_clicked();
        void on_erase_all_clicked();

        //Item List - Data model
        Glib::RefPtr<Gio::ListStore<barcode_record>> item_list;
        Glib::RefPtr<barcode_record> item_row;//Goes into the item_list

        //Packages
        guint package_count = 0;
        guint package_item_count = 0;
        Gtk::ScrolledWindow packages_scroller;
        Gtk::ListBox packaged_items;

        //Signals
        Glib::RefPtr<Gtk::SignalListItemFactory> items_in_progress_signal_factory;
        void on_item_bind ( const Glib::RefPtr<Gtk::ListItem>& list_item );
        void on_item_activate();

        //Scanner Configuration
        scan_config_window* scan_config_win;
        scan_config_record scan_config_data;

        void on_configure();
        void scan_config_on_close();

        //File Import
        Glib::RefPtr<Gtk::FileChooserNative> FileOpenOperationDialog;

        void on_import();
        void file_open_response ( int response_id );

        //File Export
        Glib::RefPtr<Gtk::FileChooserNative> FileSaveOperationDialog;

        void on_export();
        void file_save_response ( int response_id );

        void write_tab_delimited_line ( Glib::RefPtr<Gio::DataOutputStream> out, const std::string container_name, const std::string value );
        void write_xml_line ( Glib::RefPtr<Gio::DataOutputStream> out, const std::string container_name, const std::string value );
        int write_edi_856_line ( Glib::RefPtr<Gio::DataOutputStream> out, const int line_no, const std::string batch_number, const std::string value );
        void write_text_line ( Glib::RefPtr<Gio::DataOutputStream> out, const std::string value );
        void close_xml_line ( Glib::RefPtr<Gio::DataOutputStream> out );
};
#endif
