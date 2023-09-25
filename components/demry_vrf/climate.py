import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import climate
from esphome.const import CONF_ID
from . import DemryVrfComponent, DEMRY_VRF_ID

DEPENDENCIES = ['demry_vrf']

demry_vrf_ns = cg.esphome_ns.namespace('demry_vrf')
DemryVrfClimate = demry_vrf_ns.class_('DemryVrfClimate', cg.Component, climate.Climate)

CONFIG_SCHEMA = climate.CLIMATE_SCHEMA.extend({
    cv.GenerateID(): cv.declare_id(DemryVrfClimate),
    cv.GenerateID(DEMRY_VRF_ID): cv.use_id(DemryVrfComponent),
    cv.Required("idx"): cv.int_range(min=0, max=31),
}).extend(cv.COMPONENT_SCHEMA)

def to_code(config):
    paren = yield cg.get_variable(config[DEMRY_VRF_ID])
    var = cg.new_Pvariable(config[CONF_ID], paren.get_uart(), config["idx"])
    yield climate.register_climate(var, config)
    cg.add(paren.register_climate(var))
