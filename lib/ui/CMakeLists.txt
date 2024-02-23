SET(SOURCES screens/ui_oilTemp.c
    screens/ui_waterTemp.c
    screens/ui_Boost.c
    ui.c
    components/ui_comp_hook.c
    ui_helpers.c
    ui_events.c)

add_library(ui ${SOURCES})
