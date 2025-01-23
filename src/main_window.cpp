#include "main_window.hpp"
#include "barcode_record.hpp"

main_window::main_window() {
	set_title("Gautier Scanner v01");
	set_default_size(1920, 1080);

        top_frame.set_orientation(Gtk::Orientation::VERTICAL);
        set_child(top_frame);
        
        input_frame.set_orientation(Gtk::Orientation::HORIZONTAL);
        input_frame.append(barcode_field_label);
        input_frame.append(barcode_field);
        
        top_frame.set_resize_start_child(false);
        top_frame.set_start_child(input_frame);
        top_frame.set_end_child(barcodes_scroller);

        barcode_field_label.set_margin(10);
        barcode_field_label.set_text("barcode");

        barcode_field.set_margin(10);
        barcode_field.set_size_request(400);
        barcode_field.set_halign(Gtk::Align::START);
        barcode_field.set_valign(Gtk::Align::CENTER);
        barcode_field.set_hexpand(false);
        barcode_field.set_vexpand(false);

        barcodes.set_halign(Gtk::Align::FILL);
        barcodes.set_valign(Gtk::Align::FILL);
        barcodes.set_hexpand(true);
        barcodes.set_vexpand(true);
        barcodes_scroller.set_child(barcodes);
        
        /*Barcode accumulation storage*/
        barcode_list = Gio::ListStore<barcode_record>::create();

        /*Bind barcode storage to barcode list visual*/
        auto barcode_model = Gtk::SingleSelection::create(barcode_list);
        barcodes.set_model(barcode_model);

        /*Barcode column visual data binding*/
        barcode_list_signal_factory = Gtk::SignalListItemFactory::create();
        barcode_list_signal_factory->signal_setup().connect(sigc::bind(sigc::mem_fun(*this, &main_window::on_barcode_bind)));
        barcode_list_signal_factory->signal_bind().connect(sigc::bind(sigc::mem_fun(*this, &main_window::on_barcode_bind)));
        
        /*Barcode column visuals*/
        barcode_column = Gtk::ColumnViewColumn::create("Barcodes", barcode_list_signal_factory);

        barcodes.append_column(barcode_column);

        barcode_field.signal_activate().connect(sigc::bind(sigc::mem_fun(*this, &main_window::on_barcode_activate)));
        
	return;
}

void main_window::on_barcode_bind(const Glib::RefPtr<Gtk::ListItem>& list_item) {
        auto row = std::dynamic_pointer_cast<barcode_record>(list_item->get_item());

        if (!row) {
          if (list_item) {
            list_item->set_child(*Gtk::make_managed<Gtk::Label>(""));
          }
          
            return;
        }
        else {
          auto label = dynamic_cast<Gtk::Label*>(list_item->get_child());

          if (!label) {
              return;
          }

          label->set_text(row->value);
        }

        return;
}

void main_window::on_barcode_activate() {
        Glib::RefPtr<Gtk::EntryBuffer> barcode_buffer = barcode_field.get_buffer();

        if (barcode_buffer) {
            guint text_length = barcode_buffer->get_length();
            
            if(text_length > 5) {
                barcode_row = barcode_record::create(barcode_buffer->get_text());

                barcode_list->append(barcode_row);
                
                barcode_field.set_text("");
            }
        }

        return;
}
