namespace T3000Controls
{
    using System.Runtime.InteropServices;

    [Guid("B5BA92BC-CE11-4642-BC1C-204A7D698B1F")]
    [InterfaceType(ComInterfaceType.InterfaceIsIDispatch)]
    public interface ISetPointsControl
    {
        float TopValue { get; set; }
        float BottomValue { get; set; }
        float CurrentValue { get; set; }
        float TopZoneValue { get; set; }
        float BottomZoneValue { get; set; }
    }
}