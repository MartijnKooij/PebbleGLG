#include <pebble.h>

typedef struct
{
    char *store;
    char *aisle;
    char *item;
    char *quantity;
    char *sortIndex;
    char *haveItemC;
    bool haveItem;
} item;

enum {
    GLG_KEY_ERROR = 0x0,
    GLG_KEY_ACCESSKEY = 0x1,
    GLG_KEY_REQUEST = 0x2,
    GLG_KEY_ITEM = 0x3,
    GLG_KEY_listCount = 0x4
};

const char *INMYCART = "   *** IN MY CART ***";

static Window *window;
static Window *listWindow;

static MenuLayer *menuItems;

static TextLayer *textSubtitle;
static TextLayer *textTitle;

static AppTimer *timer;
static BitmapLayer *splashLayer;
static GBitmap *splashImage;

static item *listItems;

static char *accessKey;
int listCount, listItemsReceived, sectionCount;
bool hasAccessKey, jsInitCallReceived, processingList;

static void request_list(void);
static int compare_items(const void* a, const void* b);

static void select_click_handler(ClickRecognizerRef recognizer, void *context) {
    if (hasAccessKey && !processingList) {
        processingList = true;
        request_list();
        text_layer_set_text(textTitle, "GLG List");
        text_layer_set_text(textSubtitle, "Your list is loading, please wait...");
    }
}

static void click_config_provider(void *context) {
    window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
}

static uint16_t menu_get_num_sections_callback(MenuLayer *menu_layer, void *data) {
    char *store = "";
    char *aisle = "";
    sectionCount = 0;
    bool inMyCartCounted = false;
    for (int i = 0; i < listCount; i++) {
        if (listItems[i].haveItem == true) {
            if (!inMyCartCounted) {
                inMyCartCounted = true;
                sectionCount ++;
            }
        } else {
            if (strcmp(listItems[i].aisle, aisle) != 0) {
                if (strcmp(listItems[i].store, store) != 0) {
                    store = listItems[i].store;
                    sectionCount++;
                }
                aisle = listItems[i].aisle;
                sectionCount++;
            }            
        }
    }
    return sectionCount;
}

static uint16_t menu_get_num_rows_callback(MenuLayer *menu_layer, uint16_t section_index, void *data) {
    char *store = "";
    char *aisle = "";
    int sectionIndex = -1;
    int rowCount = 0;
    for (int i = 0; i < listCount; i++) {
        if (listItems[i].haveItem == true) {
            if (section_index == (sectionCount - 1)) {
                rowCount ++;
            }
        } else {
            if (strcmp(listItems[i].aisle, aisle) != 0) {
                if (strcmp(listItems[i].store, store) != 0) {
                    store = listItems[i].store;
                    sectionIndex++;
                }
                aisle = listItems[i].aisle;
                sectionIndex++;
            }
            if (section_index == sectionIndex) {
                rowCount++;
            }
            if (sectionIndex > section_index) {
                break;
            }            
        }
    }
    return rowCount;
}

static void menu_draw_header_callback(GContext* ctx, const Layer *cell_layer, uint16_t section_index, void *data) {
    char *store = "";
    char *aisle = "";
    int sectionIndex = -1;

    for (int i = 0; i < listCount; i++) {
        if (listItems[i].haveItem == true) {
            if (section_index == (sectionCount - 1)) {
                menu_cell_basic_header_draw(ctx, cell_layer, INMYCART);
            }
        } else {
            if (strcmp(listItems[i].aisle, aisle) != 0) {
                if (strcmp(listItems[i].store, store) != 0) {
                    store = listItems[i].store;
                    sectionIndex++;
                    if (section_index == sectionIndex) {
                        menu_cell_basic_header_draw(ctx, cell_layer, store);
                        break;
                    }
                }
                aisle = listItems[i].aisle;
                sectionIndex++;
                if (section_index == sectionIndex) {
                    menu_cell_basic_header_draw(ctx, cell_layer, aisle);
                    break;
                }
            }
        }
    }
}

static void menu_draw_row_callback(GContext* ctx, const Layer *cell_layer, MenuIndex *cell_index, void *data) {
    char *store = "";
    char *aisle = "";
    int sectionIndex = -1;
    int rowIndex = -1;
    for (int i = 0; i < listCount; i++) {
        if (listItems[i].haveItem == true) {
            if (cell_index->section == (sectionCount - 1)) {
                rowIndex++;
                if (cell_index->row == rowIndex) {
                    menu_cell_basic_draw(ctx, cell_layer, listItems[i].item, listItems[i].quantity, NULL);
                    break;
                }
            }
        } else {
            if (strcmp(listItems[i].aisle, aisle) != 0) {
                if (strcmp(listItems[i].store, store) != 0) {
                    store = listItems[i].store;
                    sectionIndex++;
                }
                aisle = listItems[i].aisle;
                sectionIndex++;
            }
            if (cell_index->section == sectionIndex) {
                rowIndex++;
                if (cell_index->row == rowIndex) {
                    menu_cell_basic_draw(ctx, cell_layer, listItems[i].item, listItems[i].quantity, NULL);
                    break;
                }
            }
        }
    }
}

static int16_t menu_get_header_height_callback(MenuLayer *menu_layer, uint16_t section_index, void *data) {
    return MENU_CELL_BASIC_HEADER_HEIGHT;
}

void menu_select_callback(MenuLayer *menu_layer, MenuIndex *cell_index, void *data) {
    char *store = "";
    char *aisle = "";
    int sectionIndex = -1;
    int rowIndex = -1;

    for (int i = 0; i < listCount; i++) {
        if (listItems[i].haveItem == true) {
            if (cell_index->section == (sectionCount - 1)) {
                rowIndex++;
                if (cell_index->row == rowIndex) {
                    listItems[i].haveItem = false;
                    qsort(listItems, listCount, sizeof(item), compare_items);
                    menu_layer_reload_data(menu_layer);
                    
                    break;
                }
            }
        } else {
            if (strcmp(listItems[i].store, store) != 0) {
                store = listItems[i].store;
                sectionIndex++;
            }
            if (strcmp(listItems[i].aisle, aisle) != 0) {
                aisle = listItems[i].aisle;
                sectionIndex++;
            }
            if (cell_index->section == sectionIndex) {
                rowIndex++;
                if (cell_index->row == rowIndex) {
                    listItems[i].haveItem = true;
                    qsort(listItems, listCount, sizeof(item), compare_items);
                    menu_layer_reload_data(menu_layer);

                    break;
                }
            }
        }
    }            
}

static void listwindow_load(Window *window) {
    Layer *window_layer = window_get_root_layer(window);
    GRect bounds = layer_get_bounds(window_layer);

    menuItems = menu_layer_create(bounds);

    menu_layer_set_callbacks(menuItems, NULL, (MenuLayerCallbacks) {
        .get_num_sections = menu_get_num_sections_callback,
        .get_num_rows = menu_get_num_rows_callback,
        .get_header_height = menu_get_header_height_callback,
        .draw_header = menu_draw_header_callback,
        .draw_row = menu_draw_row_callback,
        .select_click = menu_select_callback,
    });

    menu_layer_set_click_config_onto_window(menuItems, window);

    layer_add_child(window_layer, menu_layer_get_layer(menuItems));
}

static void listwindow_unload(Window *window) {
    for (int i = listCount - 1; i >= 0; i--) {
        free(listItems[i].store);
        free(listItems[i].aisle);
        free(listItems[i].item);
        free(listItems[i].quantity);
        free(listItems[i].sortIndex);
        free(listItems[i].haveItemC);
    }
    free(listItems);
    listItems = NULL;
    listCount = 0;

    menu_layer_destroy(menuItems);

    Layer *window_layer = window_get_root_layer(window);
    layer_remove_child_layers(window_layer);

    processingList = false;
}

void list_init() {
    listWindow = window_create();

    window_set_window_handlers(listWindow, (WindowHandlers) {
        .load = listwindow_load,
        .unload = listwindow_unload,
    });

    const bool animated = true;
    window_stack_push(listWindow, animated);
}

void list_deinit(void) {
    if (listWindow != NULL) {
        window_destroy(listWindow);
    }
}

static void timer_callback(void *data) {
    gbitmap_destroy(splashImage);
    bitmap_layer_destroy(splashLayer);

    if (!jsInitCallReceived) {
        text_layer_set_text(textTitle, "No bluetooth?");
        text_layer_set_text(textSubtitle, "We did not yet receive a signal from your Pebble app.");
    }
}

void out_sent_handler(DictionaryIterator *sent, void *context) {

}

void out_failed_handler(DictionaryIterator *failed, AppMessageResult reason, void *context) {

}

void in_received_handler(DictionaryIterator *received, void *context) {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "JS data received.");
    
    jsInitCallReceived = true;

    Tuple *accesskey_tuple = dict_find(received, GLG_KEY_ACCESSKEY);
    Tuple *error_tuple = dict_find(received, GLG_KEY_ERROR);
    Tuple *listCount_tuple = dict_find(received, GLG_KEY_listCount);
    Tuple *item_tuple = dict_find(received, GLG_KEY_ITEM);
    size_t index;
    char *data;
    char *store;
    char *aisle;
    char *item;
    char *quantity;
    char *haveItem;
    char *sortIndex;

    if (accesskey_tuple) {
        if (strlen(accesskey_tuple->value->cstring) == 10) {
            strncpy(accessKey, accesskey_tuple->value->cstring, 10);
            text_layer_set_text(textTitle, accessKey);
            text_layer_set_text(textSubtitle, "Click the SELECT button to load your list.");
            hasAccessKey = true;
        } else {
            text_layer_set_text(textTitle, "No Sync Key");
            text_layer_set_text(textSubtitle, "Edit the GLG settings from your Pebble app.");
        }
    }

    if (error_tuple) {
        text_layer_set_text(textTitle, "Error");
        text_layer_set_text(textSubtitle, error_tuple->value->cstring);
    }

    if (listCount_tuple) {
        listCount = listCount_tuple->value->uint8;
        text_layer_set_text(textTitle, "GLG List");
        text_layer_set_text(textSubtitle, "Receiving items...");
    }
    
    APP_LOG(APP_LOG_LEVEL_DEBUG, "Item received, processing.");

    if (item_tuple) {
        data = item_tuple->value->cstring;
        
        APP_LOG(APP_LOG_LEVEL_DEBUG, "Item data read from tuple.");
        APP_LOG(APP_LOG_LEVEL_DEBUG, "Data received: %s", data);

        //Get the store name
        index = strcspn(data, "^");
        if (index == 0) {
            char *emptyStore = "NO STORE";
            store = (char*)malloc(strlen(emptyStore));
            strcpy(store, emptyStore);
            store[8] = '\0';
        } else {
            store = (char*)malloc((sizeof(char) * index));
            strncpy(store, data, index);
            store[index] = '\0';
        }
        
        APP_LOG(APP_LOG_LEVEL_DEBUG, "Got a store name.");

        strncpy(data, &data[index + 1], strlen(data) - index);
        
        //Get the aisle name
        index = strcspn(data, "^");
        aisle = (char*)malloc((sizeof(char) * index));
        strncpy(aisle, data, index);
        aisle[index] = '\0';

        APP_LOG(APP_LOG_LEVEL_DEBUG, "Got an aisle name.");
        
        strncpy(data, &data[index + 1], strlen(data) - index);
        
        //Get the item name
        index = strcspn(data, "^");
        item = (char*)malloc((sizeof(char) * index));
        strncpy(item, data, index);
        item[index] = '\0';
        
        APP_LOG(APP_LOG_LEVEL_DEBUG, "Got an item name.");

        strncpy(data, &data[index + 1], strlen(data) - index);
                
        //Get the quantity description
        index = strcspn(data, "^");
        quantity = (char*)malloc((sizeof(char) * index));
        strncpy(quantity, data, index);
        quantity[index] = '\0';
        
        APP_LOG(APP_LOG_LEVEL_DEBUG, "Got a quantity.");

        strncpy(data, &data[index + 1], strlen(data) - index);

        //Get the sortIndex
        index = strcspn(data, "^");
        sortIndex = (char*)malloc((sizeof(char) * index));
        strncpy(sortIndex, data, index);
        sortIndex[index] = '\0';
        
        APP_LOG(APP_LOG_LEVEL_DEBUG, "Got the sort index.");

        strncpy(data, &data[index + 1], strlen(data) - index);

        //Get the haveItem flag
        index = strcspn(data, "^");
        haveItem = (char*)malloc((sizeof(char) * index));
        strncpy(haveItem, data, index);
        haveItem[index] = '\0';
        
        APP_LOG(APP_LOG_LEVEL_DEBUG, "And finally, got the have item flag.");

        if (listItems == NULL) {
            APP_LOG(APP_LOG_LEVEL_DEBUG, "Allocating list items.");
            
            listItems = malloc((listCount) * sizeof *listItems);
        }
        
        APP_LOG(APP_LOG_LEVEL_DEBUG, "Adding item to list.");
        
        listItems[listItemsReceived].store = store;
        listItems[listItemsReceived].aisle = aisle;
        listItems[listItemsReceived].item = item;
        listItems[listItemsReceived].quantity = quantity;
        listItems[listItemsReceived].sortIndex = sortIndex;
        listItems[listItemsReceived].haveItemC = haveItem;
        listItems[listItemsReceived].haveItem = strcmp(haveItem, "1") == 0;
        
        APP_LOG(APP_LOG_LEVEL_DEBUG, "Checking end of list.");
        
        listItemsReceived ++;
        if (listItemsReceived > 0 && listCount > 0 && listItemsReceived == listCount) {
            APP_LOG(APP_LOG_LEVEL_DEBUG, "Last item received, render next screen.");
            
            list_init();
            
            APP_LOG(APP_LOG_LEVEL_DEBUG, "List initialized.");

            listItemsReceived = 0;

            text_layer_set_text(textTitle, accessKey);
            text_layer_set_text(textSubtitle, "Click the SELECT button to load your list.");
        }
    }
    
    APP_LOG(APP_LOG_LEVEL_DEBUG, "JS data processed.");
}

void in_dropped_handler(AppMessageResult reason, void *context) {
}

static void request_list(void) {
    Tuplet value = TupletInteger(GLG_KEY_REQUEST, 1);

    DictionaryIterator *iter;
    app_message_outbox_begin(&iter);

    if (iter == NULL) {
        return;
    }

    dict_write_tuplet(iter, &value);
    dict_write_end(iter);

    APP_LOG(APP_LOG_LEVEL_DEBUG, "Sending list request.");

    app_message_outbox_send();
  
    APP_LOG(APP_LOG_LEVEL_DEBUG, "List request sent.");
}

static int compare_items(const void* a, const void* b) {
    int sortIndexA = atoi(((item*)a)->sortIndex);
    int sortIndexB = atoi(((item*)b)->sortIndex);

    if ((sortIndexA == sortIndexB) || (((item*)a)->haveItem == true && ((item*)b)->haveItem == true)) {
        return strcmp(((item*)a)->item, ((item*)b)->item);
    } else {
        if (sortIndexA < sortIndexB) {
            return -1;
        }
        if (sortIndexA > sortIndexB) {
            return 1;
        }
    }
    return 0;
}

static void window_load(Window *window) {
    Layer *window_layer = window_get_root_layer(window);
    GRect bounds = layer_get_bounds(window_layer);

    textTitle = text_layer_create((GRect) { .origin = { 2, 10 }, .size = { bounds.size.w - 4, 40 } });
    text_layer_set_text(textTitle, "No Sync Key");
    text_layer_set_font(textTitle, fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD));
    text_layer_set_text_alignment(textTitle, GTextAlignmentCenter);
    layer_add_child(window_layer, text_layer_get_layer(textTitle));

    textSubtitle = text_layer_create((GRect) { .origin = { 2, 50 }, .size = { bounds.size.w - 4, 80 } });
    text_layer_set_text(textSubtitle, "Edit the GLG settings from your Pebble app.");
    text_layer_set_font(textSubtitle, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));
    text_layer_set_text_alignment(textSubtitle, GTextAlignmentCenter);
    layer_add_child(window_layer, text_layer_get_layer(textSubtitle));

    splashImage = gbitmap_create_with_resource(RESOURCE_ID_SPLASH);

    splashLayer = bitmap_layer_create(bounds);
    bitmap_layer_set_bitmap(splashLayer, splashImage);
    bitmap_layer_set_alignment(splashLayer, GAlignCenter);
    layer_add_child(window_layer, bitmap_layer_get_layer(splashLayer));

    timer = app_timer_register(1500, timer_callback, NULL);
}

static void window_unload(Window *window) {
    text_layer_destroy(textSubtitle);
    text_layer_destroy(textTitle);
}

static void init(void) {
    accessKey = "          ";
    hasAccessKey = false;
    jsInitCallReceived = false;
    processingList = false;

    window = window_create();
    window_set_click_config_provider(window, click_config_provider);
    window_set_window_handlers(window, (WindowHandlers) {
        .load = window_load,
        .unload = window_unload,
    });

    app_message_register_inbox_received(in_received_handler);
    app_message_register_inbox_dropped(in_dropped_handler);
    app_message_register_outbox_sent(out_sent_handler);
    app_message_register_outbox_failed(out_failed_handler);

    const uint32_t inbound_size = 254;
    const uint32_t outbound_size = 254;
    app_message_open(inbound_size, outbound_size);

    const bool animated = true;
    window_stack_push(window, animated);
}

static void deinit(void) {
    if (hasAccessKey) {
        list_deinit();
    }
    window_destroy(window);
}

int main(void) {
    init();

    APP_LOG(APP_LOG_LEVEL_DEBUG, "Done initializing, pushed window: %p", window);

    app_event_loop();
    deinit();

    return 1;
}