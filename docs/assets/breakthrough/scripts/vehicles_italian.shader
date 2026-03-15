ab_base
{
        cull none
	qer_editorimage textures/models/vehicles/AB41/ab_base.tga
	{
		map textures/models/vehicles/AB41/ab_base.tga
		rgbGen lightingSpherical
	}
}

ab_hub
{
        nomerge  
	qer_editorimage textures/models/vehicles/AB41/ab_hub.tga
	{
		map textures/models/vehicles/AB41/ab_hub.tga
		rgbGen lightingSpherical
                tcmod rotate 5 0 -20
	}
}

ab_tire
{
        nomerge
	qer_editorimage textures/models/vehicles/AB41/ab_tire.tga
	{
		map textures/models/vehicles/AB41/ab_tire.tga
		rgbGen lightingSpherical
                tcmod scroll  0.25  0.0
	}
}
It_V_Veltro
{
	qer_editorimage textures/models/vehicles/Veltro/It_V_Veltro.tga
	{
		map textures/models/vehicles/Veltro/It_V_Veltro.tga
		rgbGen lightingSpherical
	}
}
It_V_Veltro_engine
{
	qer_editorimage textures/models/vehicles/Veltro/veltro_engine.tga
	{
		map textures/models/vehicles/Veltro/veltro_engine.tga
		rgbGen lightingSpherical
	}
}
It_V_Veltro_panel
{
	qer_editorimage textures/models/vehicles/Veltro/It_V_Veltro.tga
	{
		map textures/models/vehicles/Veltro/It_V_Veltro.tga
		rgbGen lightingSpherical
	}
	{
		map textures/models/items/pulse.tga
		blendFunc GL_SRC_ALPHA GL_ONE // this is an additive blend that uses alpha
		rgbGen wave sin 0.25 0.25 0 0.75
		alphaGen distFade 1024 512 // this makes the pulsating fade when you go away from it
	}
}
It_V_Veltro_inside
{
	qer_editorimage textures/models/vehicles/Veltro/veltro_inside.tga
	{
		map textures/models/vehicles/Veltro/veltro_inside.tga
		rgbGen lightingSpherical
	}
}
It_V_Veltro_Fly
{
	qer_editorimage textures/models/vehicles/Veltro_Fly/It_V_Veltro_Fly`.tga
	{
		map textures/models/vehicles/Veltro_Fly/It_V_Veltro_Fly.tga
		rgbGen lightingSpherical
	}
}
Prop
{
	cull none
	qer_editorimage textures/models/vehicles/Veltro_Fly/Prop.tga
	{
		clampmap textures/models/vehicles/Veltro_Fly/Prop.tga
		blendfunc blend
		rgbGen lightingSpherical
		tcmod rotate 300
	}
}
It_V_Veltrodes
{
	cull none
	qer_editorimage textures/models/vehicles/Veltro_Destroyed/It_V_Veltrodes.tga
	{
		map textures/models/vehicles/Veltro_Destroyed/It_V_Veltrodes.tga
		rgbGen lightingSpherical
	}
}
It_V_TroopTruck
{
	cull none
	qer_editorimage textures/models/vehicles/TroopTruck/It_V_TroopTruck.tga
	{
		map textures/models/vehicles/TroopTruck/It_V_TroopTruck.tga
		rgbGen lightingSpherical
	}
}
It_V_TroopTruckString
{
	cull none
	qer_editorimage textures/models/vehicles/TroopTruck/It_V_TroopTruckString.tga
	{
		map textures/models/vehicles/TroopTruck/It_V_TroopTruckString.tga
		blendfunc blend
		rgbGen lightingSpherical
	}
}
It_V_TroopTruckdes
{
	cull none
	qer_editorimage textures/models/vehicles/TroopTruck/It_V_TroopTruckdes.tga
	{
		map textures/models/vehicles/TroopTruck/It_V_TroopTruckdes.tga
		rgbGen lightingSpherical
	}
}
Af_V_ItalianJeep
{
	qer_editorimage textures/models/vehicles/Italian_Jeep/Af_V_ItalianJeep.tga
	{
		map textures/models/vehicles/Italian_Jeep/Af_V_ItalianJeep.tga
		rgbGen lightingSpherical
	}
}
CarroP40_main
{
      cull none
	qer_editorimage textures/models/vehicles/CarroP40/CarroP40_main.tga
	{
		map textures/models/vehicles/CarroP40/CarroP40_main.tga
		rgbGen lightingSpherical
	}
}
CarroP40_Wheel1
{
	qer_editorimage textures/models/vehicles/CarroP40/CarroP40_Wheel1.tga
   nomerge
	{
		map textures/models/vehicles/CarroP40/CarroP40_Wheel1.tga
		rgbGen lightingSpherical
		tcmod rotate 5 0 20
	}
}
CarroP40_Wheel2
{
	qer_editorimage textures/models/vehicles/CarroP40/CarroP40_Wheel2.tga
   nomerge
	{
		map textures/models/vehicles/CarroP40/CarroP40_Wheel2.tga
		rgbGen lightingSpherical
		tcmod rotate 5 0 20
	}
}
CarroP40_Wheel3
{
	qer_editorimage textures/models/vehicles/CarroP40/CarroP40_Wheel3.tga
   nomerge
	{
		map textures/models/vehicles/CarroP40/CarroP40_Wheel3.tga
		rgbGen lightingSpherical
		tcmod rotate 5 0 20
	}
}
CarroP40_Wheel4
{
   cull none
   qer_editorimage textures/models/vehicles/CarroP40/CarroP40_Wheel4.tga
   nomerge
	{
		map textures/models/vehicles/CarroP40/CarroP40_Wheel4.tga
                alphaFunc GE128
		rgbGen lightingSpherical
		tcmod rotate 5 0 20
	}
}
CarroP40_Tracks
{
   cull none
   qer_editorimage textures/models/vehicles/CarroP40/CarroP40_Tracks.tga
   nomerge
	{
      map textures/models/vehicles/CarroP40/CarroP40_Tracks.tga
      alphaFunc GE128
      rgbGen lightingSpherical
      tcmod scroll -0 1
	}
}

CarroP40_main_D_M1
{
        cull none
	qer_editorimage textures/models/vehicles/CarroP40/CarroP40_main_D.tga
	{
		map textures/models/vehicles/CarroP40/CarroP40_main_D.tga
		rgbGen lightingSpherical
	}
}
OilTanker_01
{
	qer_editorimage textures/models/vehicles/OilTanker/OilTanker1.tga
	{
		map textures/models/vehicles/OilTanker/OilTanker1.tga
		rgbGen lightingSpherical
	}
}
OilTanker_02
{
	qer_editorimage textures/models/vehicles/OilTanker/OilTanker2.tga
	{
		map textures/models/vehicles/OilTanker/OilTanker2.tga
		rgbGen lightingSpherical
	}
}
OilTanker_01_des
{
	qer_editorimage textures/models/vehicles/OilTanker/OilTanker1_des.tga
	{
		map textures/models/vehicles/OilTanker/OilTanker1_des.tga
		rgbGen lightingSpherical
	}
}
OilTanker_02_des
{
	cull none
	qer_editorimage textures/models/vehicles/OilTanker/OilTanker2_des.tga
	{
		map textures/models/vehicles/OilTanker/OilTanker2_des.tga
		rgbGen lightingSpherical
	}
}
AX_V_Opal_Main
{
	qer_editorimage textures/models/vehicles/opel_w/Opel.tga
	{
		map textures/models/vehicles/opel_w/Opel.tga
		rgbGen lightingSpherical
	}
}
AX_V_Opal_Main_doublesided
{
	cull none
	qer_editorimage textures/models/vehicles/opel_w/Opel.tga
	{
		map textures/models/vehicles/opel_w/Opel.tga
		rgbGen lightingSpherical
	}
}
AX_V_Opal_Glass
{
	cull none
	qer_editorimage textures/models/vehicles/AX_V_Opal/opalglass.tga
	{
		map textures/models/vehicles/AX_V_Opal/opalglass.tga
		blendFunc blend
		rgbGen lightingSpherical
	}
}
AX_V_Opal_Wheel1
{
	qer_editorimage textures/models/vehicles/opel_w/AX_V_Opal_Wheel1.tga
	{
		map textures/models/vehicles/opel_w/AX_V_Opal_Wheel1.tga
		rgbGen lightingSpherical
	}
}
AX_V_Opal_Wheel2
{
	qer_editorimage textures/models/vehicles/opel_w/AX_V_Opal_Wheel2.tga
	{
		map textures/models/vehicles/opel_w/AX_V_Opal_Wheel2.tga
		rgbGen lightingSpherical
	}
}
AX_V_Opal_Tread
{
	qer_editorimage textures/models/vehicles/opel_w/AX_V_Opal_Tread.tga
	{
		map textures/models/vehicles/opel_w/AX_V_Opal_Tread.tga
		rgbGen lightingSpherical
	}
}
CarroP40_main_D
{
      cull none
	qer_editorimage textures/models/vehicles/CarroP40/CarroP40_main_D.tga
	{
		map textures/models/vehicles/CarroP40/CarroP40_main_D.tga
		rgbGen lightingSpherical
	}
}
CarroP40_Wheel1_D
{
	qer_editorimage textures/models/vehicles/CarroP40/CarroP40_Wheel1_D.tga
	{
		map textures/models/vehicles/CarroP40/CarroP40_Wheel1_D.tga
		rgbGen lightingSpherical
	}
}
CarroP40_Wheel2_D
{
	qer_editorimage textures/models/vehicles/CarroP40/CarroP40_Wheel2_D.tga
	{
		map textures/models/vehicles/CarroP40/CarroP40_Wheel2_D.tga
		rgbGen lightingSpherical
	}
}
CarroP40_Wheel3_D
{
	qer_editorimage textures/models/vehicles/CarroP40/CarroP40_Wheel3_D.tga
	{
		map textures/models/vehicles/CarroP40/CarroP40_Wheel3_D.tga
		rgbGen lightingSpherical
	}
}
CarroP40_Wheel4_D
{
	cull none
        qer_editorimage textures/models/vehicles/CarroP40/CarroP40_Wheel4_D.tga
	{
		map textures/models/vehicles/CarroP40/CarroP40_Wheel4_D.tga
                alphaFunc GE128
		rgbGen lightingSpherical
	}
}
CarroP40_Tracks_D
{
	cull none
        qer_editorimage textures/models/vehicles/CarroP40/CarroP40_Tracks_D.tga
	{
		map textures/models/vehicles/CarroP40/CarroP40_Tracks_D.tga
                alphaFunc GE128
		rgbGen lightingSpherical
	}
}
BMWbike
{
	cull none
        qer_editorimage textures/models/vehicles/BMWbike/Sc_V_BMWbike.tga
	{
		map textures/models/vehicles/BMWbike/Sc_V_BMWbike.tga
                alphaFunc GE128
		rgbGen lightingSpherical
	}
}
BMWbike_wheel
{
	cull none
	noMerge
        qer_editorimage textures/models/vehicles/BMWbike/bmwbike_wheel.jpg
	{
		map textures/models/vehicles/BMWbike/bmwbike_wheel.jpg
		rgbGen lightingSpherical
		tcmod rotate 5 0 -60
	}
}
BMWbike_wheel_static
{
	cull none
        qer_editorimage textures/models/vehicles/BMWbike/bmwbike_wheel.jpg
	{
		map textures/models/vehicles/BMWbike/bmwbike_wheel.jpg
		rgbGen lightingSpherical
	}
}
BMWbike_hubcap
{
	cull none
	noMerge
        qer_editorimage textures/models/vehicles/BMWbike/bmwbike_hubcap.jpg
	{
		map textures/models/vehicles/BMWbike/bmwbike_hubcap.jpg
		rgbGen lightingSpherical
		tcmod rotate 5 0 -60
	}
}
BMWbike_hubcap_static
{
	cull none
        qer_editorimage textures/models/vehicles/BMWbike/bmwbike_hubcap.jpg
	{
		map textures/models/vehicles/BMWbike/bmwbike_hubcap.jpg
		rgbGen lightingSpherical
	}
}
BMWbike_rim
{
	cull none
	noMerge
        qer_editorimage textures/models/vehicles/BMWbike/bmwbike_rim.jpg
	{
		map textures/models/vehicles/BMWbike/bmwbike_rim.jpg
		rgbGen lightingSpherical
		tcmod scroll -2.4 0
	}
}
BMWbike_rim_static
{
	cull none
        qer_editorimage textures/models/vehicles/BMWbike/bmwbike_rim.jpg
	{
		map textures/models/vehicles/BMWbike/bmwbike_rim.jpg
		rgbGen lightingSpherical
	}
}