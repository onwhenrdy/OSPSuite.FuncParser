using System.Runtime.InteropServices;

namespace OSPSuite.FuncParser
{
   public class DimensionInformation
   {
      // ReSharper disable once UnusedMember.Global
      public DimensionInformation()
      {
         LengthExponent = 0;
         MassExponent = 0;
         TimeExponent = 0;
         ElectricCurrentExponent = 0;
         TemperatureExponent = 0;
         AmountExponent = 0;
         LuminousIntensityExponent = 0;
      }

      public DimensionInformation(DimensionInformation dimensionInformation)
      {
         LengthExponent = dimensionInformation.LengthExponent;
         MassExponent = dimensionInformation.MassExponent;
         TimeExponent = dimensionInformation.TimeExponent;
         ElectricCurrentExponent = dimensionInformation.ElectricCurrentExponent;
         TemperatureExponent = dimensionInformation.TemperatureExponent;
         AmountExponent = dimensionInformation.AmountExponent;
         LuminousIntensityExponent = dimensionInformation.LuminousIntensityExponent;
      }

      public DimensionInformation(double lengthExponent, double massExponent, double timeExponent,
         double electricCurrentExponent, double temperatureExponent, double amountExponent, double luminousIntensityExponent)
      {
         LengthExponent = lengthExponent;
         MassExponent = massExponent;
         TimeExponent = timeExponent;
         ElectricCurrentExponent = electricCurrentExponent;
         TemperatureExponent = temperatureExponent;
         AmountExponent = amountExponent;
         LuminousIntensityExponent = luminousIntensityExponent;
      }

      internal DimensionInformation(DimensionInformationStructure dimensionInformation)
      {
         LengthExponent = dimensionInformation.LengthExponent;
         MassExponent = dimensionInformation.MassExponent;
         TimeExponent = dimensionInformation.TimeExponent;
         ElectricCurrentExponent = dimensionInformation.ElectricCurrentExponent;
         TemperatureExponent = dimensionInformation.TemperatureExponent;
         AmountExponent = dimensionInformation.AmountExponent;
         LuminousIntensityExponent = dimensionInformation.LuminousIntensityExponent;
      }

      public double LengthExponent { get; set; }
      public double MassExponent { get; set; }
      public double TimeExponent { get; set; }
      public double ElectricCurrentExponent { get; set; }
      public double TemperatureExponent { get; set; }
      public double AmountExponent { get; set; }
      public double LuminousIntensityExponent { get; set; }

      public override bool Equals(object obj)
      {
         return (obj is DimensionInformation dimensionInformation) &&
                LengthExponent.Equals(dimensionInformation.LengthExponent) &&
                MassExponent.Equals(dimensionInformation.MassExponent) &&
                TimeExponent.Equals(dimensionInformation.TimeExponent) &&
                ElectricCurrentExponent.Equals(dimensionInformation.ElectricCurrentExponent) &&
                TemperatureExponent.Equals(dimensionInformation.TemperatureExponent) &&
                AmountExponent.Equals(dimensionInformation.AmountExponent) &&
                LuminousIntensityExponent.Equals(dimensionInformation.LuminousIntensityExponent);
      }

      public override int GetHashCode()
      {
         unchecked
         {
            var hashCode = LengthExponent.GetHashCode();
            hashCode = (hashCode * 397) ^ MassExponent.GetHashCode();
            hashCode = (hashCode * 397) ^ TimeExponent.GetHashCode();
            hashCode = (hashCode * 397) ^ ElectricCurrentExponent.GetHashCode();
            hashCode = (hashCode * 397) ^ TemperatureExponent.GetHashCode();
            hashCode = (hashCode * 397) ^ AmountExponent.GetHashCode();
            hashCode = (hashCode * 397) ^ LuminousIntensityExponent.GetHashCode();
            return hashCode;
         }
      }

   }

   public class QuantityDimensionInformation : DimensionInformation
   {
      // ReSharper disable once UnusedMember.Global
      public QuantityDimensionInformation(string quantityName, DimensionInformation dimensionInformation) : base(dimensionInformation)
      {
         QuantityName = quantityName;
      }

      public QuantityDimensionInformation(string quantityName, double lengthExponent, double massExponent,
         double timeExponent,
         double electricCurrentExponent, double temperatureExponent, double amountExponent,
         double luminousIntensityExponent)
         : base(lengthExponent, massExponent, timeExponent, electricCurrentExponent, temperatureExponent,
            amountExponent, luminousIntensityExponent)
      {
         QuantityName = quantityName;
      }

      public string QuantityName { get; set; }
   }

   [StructLayout(LayoutKind.Sequential)]
   internal struct QuantityDimensionInformationStructure
   {
      public double LengthExponent;
      public double MassExponent;
      public double TimeExponent;
      public double ElectricCurrentExponent;
      public double TemperatureExponent;
      public double AmountExponent;
      public double LuminousIntensityExponent;
      public string QuantityName;

      public QuantityDimensionInformationStructure(QuantityDimensionInformation dimensionInformation)
      {
         LengthExponent = dimensionInformation.LengthExponent;
         MassExponent = dimensionInformation.MassExponent;
         TimeExponent = dimensionInformation.TimeExponent;
         ElectricCurrentExponent = dimensionInformation.ElectricCurrentExponent;
         TemperatureExponent = dimensionInformation.TemperatureExponent;
         AmountExponent = dimensionInformation.AmountExponent;
         LuminousIntensityExponent = dimensionInformation.LuminousIntensityExponent;
         QuantityName = dimensionInformation.QuantityName;
      }
   }

   [StructLayout(LayoutKind.Sequential)]
   internal struct DimensionInformationStructure
   {
      public double LengthExponent;
      public double MassExponent;
      public double TimeExponent;
      public double ElectricCurrentExponent;
      public double TemperatureExponent;
      public double AmountExponent;
      public double LuminousIntensityExponent;
   }
}