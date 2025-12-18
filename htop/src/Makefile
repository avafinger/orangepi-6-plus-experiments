# Makefile for htop

include ../Rules.mak

CFLAGS  = $(BIN_CFLAGS)
SRCDIR	= ${PWD}
#OBJECTS  := $(patsubst %.c,%.o,$(wildcard *.c))
OBJECTS = htop.o Action.o UptimeMeter.o Header.o Affinity.o FunctionBar.o ProcessList.o MetersPanel.o CheckItem.o CategoriesPanel.o ColumnsPanel.o \
		BatteryMeter.o \
		CpuTempMeter.o \
		IOPriorityPanel.o \
		Platform.o \
		IOPriority.o \
		LinuxProcessList.o \
		LinuxProcess.o \
		LinuxCRT.o \
		Battery.o \
		Settings.o \
		CpuVcoreMeter.o \
		ListItem.o \
		AffinityPanel.o \
		EnvScreen.o \
		InfoScreen.o \
		TasksMeter.o \
		HostnameMeter.o \
		LoadAverageMeter.o \
		DisplayOptionsPanel.o \
		Hashtable.o \
		OpenFilesScreen.o \
		ColorsPanel.o \
		Meter.o \
		XAlloc.o \
		Object.o \
		TraceScreen.o \
		UsersTable.o \
		SwapMeter.o \
		MainPanel.o \
		IncSet.o \
		AvailableMetersPanel.o \
		ClockMeter.o \
		MemoryMeter.o \
		Panel.o \
		AvailableColumnsPanel.o \
		ScreenManager.o \
		StringUtils.o \
		CpuFreqMeter.o \
		RichString.o \
		CRT.o \
		Process.o \
		Vector.o \
		CPUMeter.o \
		SignalsPanel.o \
		interfaces.o \
		OS_Meter.o \
		Eth0_Meter.o \
		Eth1_Meter.o \
		Wlan0_Meter.o \
		Wlan1_Meter.o \
		Kernel_Meter.o \
		Armbian_Meter.o \
		BlockDevice_ioStatsMeter.o \
		GpuTempMeter.o \
		Eth0_StatsMeter.o \
		Eth1_StatsMeter.o \
		Wlan0_StatsMeter.o \
		mountpoint.o \
		fsUsage_Meter.o \
		Users_Meter.o \

TARGET  = $(BIN_TARGETDIR)/htop

all: _htop_release

_htop_release: $(OBJECTS)
	@echo "\nBuilding RELEASE $(TARGET)"
	@echo "$(SRCDIR)"
	$(CC) -o $(TARGET) $(OBJECTS) $(BIN_LDFLAGS_T)
	@echo "** $(SRCDIR)\n"

$(OBJECTS): htop.h
Action.o: Action.h
UptimeMeter.o: UptimeMeter.h
Header.o: Header.h
Affinity.o: Affinity.h
FunctionBar.o: FunctionBar.h
ProcessList.o: ProcessList.h
MetersPanel.o: MetersPanel.h
CheckItem.o: CheckItem.h
CategoriesPanel.o: CategoriesPanel.h
ColumnsPanel.o: ColumnsPanel.h
BatteryMeter.o: BatteryMeter.h
CpuTempMeter.o: CpuTempMeter.h
CpuVcoreMeter.o: CpuVcoreMeter.h
IOPriorityPanel.o: IOPriorityPanel.h
Platform.o: Platform.h
IOPriority.o: IOPriority.h
LinuxProcessList.o: LinuxProcessList.h
LinuxProcess.o: LinuxProcess.h
LinuxCRT.o: LinuxCRT.h
Battery.o: Battery.h
Settings.o: Settings.h
ListItem.o: ListItem.h
AffinityPanel.o: AffinityPanel.h
EnvScreen.o: EnvScreen.h
InfoScreen.o: InfoScreen.h
TasksMeter.o: TasksMeter.h
HostnameMeter.o: HostnameMeter.h
LoadAverageMeter.o: LoadAverageMeter.h
DisplayOptionsPanel.o: DisplayOptionsPanel.h
Hashtable.o: Hashtable.h
OpenFilesScreen.o: OpenFilesScreen.h
ColorsPanel.o: ColorsPanel.h
Meter.o: Meter.h
XAlloc.o: XAlloc.h
Object.o: Object.h
TraceScreen.o: TraceScreen.h
UsersTable.o: UsersTable.h
SwapMeter.o: SwapMeter.h
MainPanel.o: MainPanel.h
IncSet.o: IncSet.h
AvailableMetersPanel.o: AvailableMetersPanel.h
ClockMeter.o: ClockMeter.h
MemoryMeter.o: MemoryMeter.h
Panel.o: Panel.h
AvailableColumnsPanel.o: AvailableColumnsPanel.h
ScreenManager.o: ScreenManager.h
StringUtils.o: StringUtils.h
CpuFreqMeter.o: CpuFreqMeter.h
RichString.o: RichString.h
htop.o: htop.h
CRT.o: CRT.h
Process.o: Process.h
Vector.o: Vector.h
CPUMeter.o: CPUMeter.h
SignalsPanel.o: SignalsPanel.h
BlockDevice_ioStatsMeter.o: BlockDevice_ioStatsMeter.h
interfaces.o: interfaces.h
GpuTempMeter.o: GpuTempMeter.h
Eth0_StatsMeter.o: Eth0_StatsMeter.h
Eth1_StatsMeter.o: Eth1_StatsMeter.h
Wlan0_StatsMeter.o: Wlan0_StatsMeter.h
mountpoint.o: mountpoint.h
fsUsage_Meter.o: fsUsage_Meter.h
Users_Meter.o: Users_Meter.h

clean:
	rm -f $(TARGET) $(OBJECTS) *~
